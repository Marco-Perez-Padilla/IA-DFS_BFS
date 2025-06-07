/**
** Universidad de La Laguna
** Escuela Superior de Ingenieria y Tecnologia
** Grado en Ingenieria Informatica
** Asignatura: Inteligencia Artificial
** Curso: 3º
** Practica 1: Busquedas no informadas
** Autor: Marco Pérez Padilla
** Correo: alu0101469348@ull.edu.es
** Fecha: 27/09/2024

** Archivo first_searchs.cc: Implementación de la clase FirstSearchs para manejar los recorridos en un grafo
**      
** Referencias:
**      Enlaces de interes

** Historial de revisiones:
**      27/09/2024 - Creacion del código
**      15/01/2025 - Arreglo de las funciones del BFS
**      31/05/2025 - Arreglo de DFS y refactorizacion del codigo
**/

#include "first_searchs.h"

#include <cstdlib> 

/**
 * @brief method to clear Graph
 */
void FirstSearchs::destroy() {
	for (unsigned i=0; i< number_nodes_; i++) {
	  adyacent_list_[i].clear();
	}
	adyacent_list_.clear();
}


/**
 * @brief method to build a graph
 * @param name of the file to be processed
 * @param int to show if any error occured while building the graph
 */
void FirstSearchs::build(char namefile[85], int &error_flag) {
  ListNode list_node;   // Initializing the list
	std::ifstream textfile;
	textfile.open(namefile); // Reading the file

  // If file could be opened
	if (textfile.is_open()) {
		unsigned i, j;
    std::string line;
    double costs;
		textfile >> (long unsigned int&) number_nodes_;  // Reading the number of nodes
    list_node.predecesors.resize(number_nodes_ * number_nodes_);   // Resizing the predecesors vector

		adyacent_list_.resize(number_nodes_);  // Resizing the list
		for (i = 0; i < number_nodes_; i++) {    // For each node
      for (j = i + 1; j < number_nodes_; j++) {  // For each of the rest of nodes
        textfile >> line; // Read the line
        std::replace(line.begin(), line.end(), ',', '.'); // Replace commas with points
        std::stringstream weight_line (line); 
        weight_line >> costs;  // Read the weights
        if (costs != -1.00) {  // If weight different than -1.00, there is a connection between i and j
          list_node.node = j;
          list_node.cost = static_cast<int>(costs); // Transforming weight to an int
          // Associating the list
          adyacent_list_[i].push_back(list_node);
          list_node.node = i;
          adyacent_list_[j].push_back(list_node);
        }
      }
    }
    textfile.close();
    error_flag = 0;
  } else {
    error_flag = 1; // If file couldn't be opened
  }
}


/**
 * @brief destructor, uses destroy method
 */
FirstSearchs::~FirstSearchs() {
	destroy();
}


/**
 * @brief constructor, uses build method
 * @param name of the file to be processed
 * @param int to show if any error occured while building the graph
 */
FirstSearchs::FirstSearchs(char namefile[85], int &error_flag) {
	build (namefile, error_flag);
}


/**
 * @brief method to update graphs
 * @param name of the file to be processed
 * @param int to show if any error occured while building the graph
 */
void FirstSearchs::update(char namefile[85], int &error_flag) {
  destroy();
  build(namefile, error_flag);
}


/**
 * @brief Method to show the relation of nodes and weights in a list
 * @param adyacent_list_ to be shown
 */
void FirstSearchs::ShowList(const std::vector<AdyacentListNode>& list) {
  for (size_t i = 0; i < list.size(); ++i) {
    std::cout << "Node " << i + 1 << ": ";
    for (const auto& list_node : list[i]) {
      std::cout << "(" << list_node.node + 1 << "," << list_node.cost << ") ";
    }
    std::cout << std::endl;
  }
}


/**
 * @brief Method to show adyacent list using the previous method
 */
void FirstSearchs :: ShowLists () {
  std::cout << "Adyacency list:" << std::endl;
  ShowList(adyacent_list_);
  return;
}
    

/**
 * @brief Recursive method that does a depth first research from an origin to a destiny
 * @param int type, origin node. Node where the search will begin
 * @param int type, destiny node. Node where the search will end
 * @param vector of lists. Matrix with the relations between nodes
 * @param vector that tracks the current branch
 * @param vector of ints sucesors. Stores the sucesors
 * @param vector of ints distance. Stores the cost
 * @param vector of ints generated. Stores the generated nodes
 * @param vector of ints inspected. Stores the inspected nodes
 * @param int iteration. Used for initializating and to keep a record of the iterations
 * @param bool found. Used to sign if the destiny has been found
 * @param ofstream output file
 */
void FirstSearchs::RecursiveDFS(unsigned current_node, unsigned destiny, std::vector<AdyacentListNode>& adyacent_list_, std::vector<unsigned>& current_path, 
                          std::vector<unsigned>& sucesors, std::vector<unsigned>& distances, std::vector<unsigned>& generated_nodes, 
                          std::vector<unsigned>& inspected_nodes, unsigned& iteration, bool& found, std::ofstream& file) {

  // Store the path (each branch)
  current_path.push_back(current_node);
    
  // Generating the neighbours
  for (const auto& edge : adyacent_list_[current_node]) {
    unsigned neighbour = edge.node;
        
    if (std::find(current_path.begin(), current_path.end(), neighbour) != current_path.end()) 
      continue;  // If it is on the current path (current branch), skip it. This allows to repeat nodes in different paths (branches)
        
    generated_nodes.push_back(neighbour + 1);
  }
    
  // Inspect current
  inspected_nodes.push_back(current_node + 1);
  ShowIteration(iteration++, generated_nodes, inspected_nodes, file);
    
  // 4. Process sucesors
  for (const auto& edge : adyacent_list_[current_node]) {
    if (found) return;
        
    unsigned neighbour = edge.node;
        
    // Skip if it's already on the branch
    if (std::find(current_path.begin(), current_path.end(), neighbour) != current_path.end()) 
      continue;
        
    // Update information
    sucesors[neighbour] = current_node;
    distances[neighbour] = distances[current_node] + edge.cost;
        
    // Check if it's the destiny. If it is, inspect it
    if (neighbour == destiny) {
      inspected_nodes.push_back(neighbour + 1);
      ShowIteration(iteration++, generated_nodes, inspected_nodes, file);
      found = true;
      return;
    }
        
    // Recursive call
    RecursiveDFS(neighbour, destiny, adyacent_list_, current_path, sucesors, distances, 
      generated_nodes, inspected_nodes,iteration, found, file);
  }
    
  // 5. Backtracking
  current_path.pop_back();
}


/**
 * @brief Method to specify the dfs
 * @param ofstream file to be written
 */
void FirstSearchs::DepthSearch(std::ofstream& file) {
  unsigned initial_node, destiny;
  bool error = false;
  // Ask the user for origin and destiny
  do {
    std::cout << "Doing a Depth Search. Choose an initial node: [1-" << number_nodes_ << "]: ";
    if (!(std::cin >> initial_node)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cerr << "Invalid entry" << std::endl;
      error = true;
      continue;
    }
    std::cout << "Choose the destination node: [1-" << number_nodes_ << "]: ";
    if (!(std::cin >> destiny)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cerr << "Invalid entry" << std::endl;
      error = true;
      continue;
    }
    // Manage range errors in user's answer
    if (initial_node < 1 || initial_node > number_nodes_ || destiny < 1 || destiny > number_nodes_) {
      std::cerr << "Node not valid. It must be a number between 1 and " << number_nodes_ << std::endl;
      error = true;
    } else {
      error = false;
    }
  } while (error == true);
  
  // Adjust for internal work
  initial_node--;
  destiny--;

  // Print basic information
  file << "Number of nodes of the graph: " << number_nodes_ << std::endl;
  file << "Origin node: " << initial_node + 1 << std::endl;
  file << "Destiny node: " << destiny + 1 << std::endl;
  file << std::endl;

  // Initializing structures
  std::vector<unsigned> sucesors(number_nodes_, -1);
  std::vector<unsigned> distance(number_nodes_, 0);
  std::vector<unsigned> generated_nodes;
  std::vector<unsigned> inspected_nodes;
  std::vector<unsigned> path;
  std::set<unsigned> global_generated_nodes;
  unsigned iteration = 1;
  bool found = false;

  // First node
  global_generated_nodes.insert(initial_node);
  generated_nodes.push_back(initial_node + 1);
  ShowIteration(iteration++, generated_nodes, inspected_nodes, file);

  RecursiveDFS(initial_node, destiny, adyacent_list_, path, sucesors, distance,  
    generated_nodes, inspected_nodes, iteration, found, file);

  // If the search has finished and found a path
  if (found) {
    file << "Found path from " << initial_node + 1 << " to " << destiny + 1 << ": ";
    // Show path
    ShowPathDFS(initial_node, destiny, sucesors, file);
    file << std::endl;
    // Show cost
    file << "Cost: " << distance[destiny] << std::endl;
  } else { // Otherwise, there is no path
    file << "Path not found from " << initial_node + 1 << " to " << destiny + 1 << std::endl;
  }

  file.close();
}


/**
 * @brief Recursive method that does a Breadth-first search from an origin to a destiny
 * @param int type, origin node. Node where the search will begin
 * @param int type, destiny node. Node where the search will end
 * @param int type, current node that is being processed
 * @param queue of nodes
 * @param vector of bools visited. Associates each node a bool if it's been visited or not
 * @param vector of ints sucesors. Stores the sucesors
 * @param vector of ints distance. Stores the cost
 * @param vector of lists. Matrix with the relations between nodes
 * @param vector of ints generated. Stores the generated nodes
 * @param vector of ints inspected. Stores the inspected nodes
 * @param bool found. Used to sign if the destiny has been found
 * @param int iteration. Used for initializating and to keep a record of the iterations
 * @param Struct to organize the levels: level vector -> branch vector -> pair of set and vector.
 * @param vector of nodes that will store the path that detected first the destiny
 * @param ofstream output file
 */
void FirstSearchs::RecursiveBFS(unsigned origin_node, unsigned destiny, unsigned current_node, std::queue<unsigned>& queue, 
                          std::vector<bool>& visited, std::vector<unsigned>& sucesors, std::vector<unsigned>& distance, 
                          std::vector<AdyacentListNode>& list, std::vector<unsigned>& generated_nodes, std::vector<unsigned>& inspected_nodes,
                          bool& found, unsigned& iteration, std::vector<std::vector<std::pair<std::set<unsigned>, std::vector<unsigned>>>>& levels, 
                          std::vector<unsigned>& path, std::ofstream& file) {

  // If our struct is empty
  if (levels.empty()) {
    // Initialization of the first level. Origin
    levels.push_back({{{origin_node}, {origin_node}}});
    generated_nodes.push_back(origin_node + 1);
    ShowIteration(iteration++, generated_nodes, inspected_nodes, file);
  }

  // If the queue is empty, or it's been found, finish the search
  if (queue.empty() || found) {
    return;
  }

  // We get the next node from the front of the queue
  unsigned next_nodo = queue.front();
  queue.pop();

  // Creating the new level where the sucesors of our node will be located
  std::vector<std::pair<std::set<unsigned>, std::vector<unsigned>>> new_level;

  // Processing the current level. For each branch in the last level
  for (const auto& rama : levels.back()) {
    const auto& visited_nodes = rama.first; //Set that stores the nodes visited in each branch
    const auto& current_nodes = rama.second; //Vector that mantains the order of the branch

    // We inspectionate the last node of the vector in each branch
    unsigned int current_node = current_nodes.back();
    inspected_nodes.push_back(current_node + 1);

    // If it's the destiny, finish the search
    if (current_node == destiny) {
      found = true;
      path = current_nodes;
      
      ShowIteration(iteration++, generated_nodes, inspected_nodes, file);
      return;
    }

    // For each node sucesor
    for (unsigned j = 0; j < list[current_node].size(); ++j) {
      unsigned neighbour = list[current_node][j].node;

      // If the sucesor is not in the branch, then
      if (visited_nodes.find(neighbour) == visited_nodes.end()) {
        //Create new set representing the new branch we've just discovered, and do so with a vector as well
        std::set<unsigned> new_set = visited_nodes;
        new_set.insert(neighbour);

        std::vector<unsigned> new_vector = current_nodes;
        new_vector.push_back(neighbour);

        //Add the new branch to our new level
        new_level.emplace_back(new_set, new_vector);
        
        // Generate it
        generated_nodes.push_back(neighbour + 1);
        // Add it to the queue
        queue.push(neighbour);
        // Update the sucesor
        sucesors[neighbour] = current_node;

        // If it hasn't been visited
        if (!visited[neighbour]) {
          visited[neighbour] = true; 
          // Update distance
          distance[neighbour] = distance[current_node] + list[current_node][j].cost;
        }
      }    
    }
    ShowIteration(iteration++, generated_nodes, inspected_nodes, file);
  }

  // If the new level is not empty, add it to our structure
  if (!new_level.empty()) {
    levels.push_back(new_level);
  }

  // Recursive call
  RecursiveBFS(origin_node, destiny, next_nodo, queue, visited, sucesors, distance, 
    list, generated_nodes, inspected_nodes, found, iteration, levels, path, file);
}


/**
 * @brief Method to specify the bfs
 * @param ofstream name of the output file
 */
void FirstSearchs::BreadthSearch(std::ofstream& file) {
  unsigned initial_node, destiny;
  bool error = false;
  // Ask the user for origin and destiny
  do {
    std::cout << "Doing a Depth Search. Choose an initial node: [1-" << number_nodes_ << "]: ";
    if (!(std::cin >> initial_node)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cerr << "Invalid entry" << std::endl;
      error = true;
      continue;
    }
    std::cout << "Choose the destination node: [1-" << number_nodes_ << "]: ";
    if (!(std::cin >> destiny)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cerr << "Invalid entry" << std::endl;
      error = true;
      continue;
    }
    // Manage range errors in user's answer
    if (initial_node < 1 || initial_node > number_nodes_ || destiny < 1 || destiny > number_nodes_) {
      std::cerr << "Node not valid. It must be a number between 1 and " << number_nodes_ << std::endl;
      error = true;
    } else {
      error = false;
    }
  } while (error == true);

  // Adjust it for internal purposes
  initial_node--;
  destiny--;
  
  // Print basic information
  file << "Number of nodes of the graph: " << number_nodes_ << std::endl;
  file << "Origin node: " << initial_node + 1 << std::endl;
  file << "Destiny node: " << destiny + 1 << std::endl;
  file << std::endl;

  // Initializing structures
  std::vector<bool> visited(number_nodes_, false);          
  std::vector<unsigned> sucesors(number_nodes_, -1);            
  std::vector<unsigned> distance(number_nodes_, 0);                 
  std::queue<unsigned> queue;                     
  std::vector<unsigned> generated_nodes;
  std::vector<unsigned> inspected_nodes;
  std::vector<std::vector<std::pair<std::set<unsigned>, std::vector<unsigned>>>> levels;
  std::vector<unsigned> path;
  bool found = false;
  unsigned iteration = 1;  

  // Prepare to call the search
  visited[initial_node] = true;
  sucesors[initial_node] = initial_node;
  distance[initial_node] = 0;
  queue.push(initial_node);

  RecursiveBFS(initial_node, destiny, initial_node, queue, visited, sucesors, distance, 
    adyacent_list_, generated_nodes, inspected_nodes, found, iteration, levels, path, file);

  // If the search has finished and found a path
  if (found) {
    file << "Found path from " << initial_node + 1 << " to " << destiny + 1 << ": ";
    // Show path
    ShowPathBFS(path, file);
    file << std::endl;
    // Show cost
    file << "Cost: " << distance[destiny] << std::endl;
  } else { // Otherwise, there is no path
    file << "Path not found from " << initial_node + 1 << " to " << destiny + 1 << std::endl;
  }
  file.close();
}


/**
 * @brief function that waits for the user to press any key
 */
void pressanykey() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
  std::cin.get(); 
}

/**
 * @brief function to clear the standard ouput
 */
void clrscr() {
  system("clear");
}


/**
 * @brief Function that shows a path for a DFS
 * @param int type s, start
 * @param int type i, finish
 * @param vector of sucesors
 * @param ofstream name of the file to be written in
 */
void ShowPathDFS(unsigned s, unsigned i, std::vector<unsigned> sucesors, std::ofstream& file) {
  if (i != s) {
    ShowPathDFS(s,sucesors[i],sucesors, file);
    file << " - " << i+1;
  } else {
    file << i+1;
  }
}


/**
 * @brief Function that shows a path for a BFS
 * @param vector result path
 * @param ofstream name of the file to be written in
 */
void ShowPathBFS(const std::vector<unsigned>& path, std::ofstream& file) {
  if (path.empty()) {
    file << "Error: Empty path" << std::endl;
    return;
  }
    
  for (size_t i = 0; i < path.size(); ++i) {
    file << path[i] + 1;  
    if (i < path.size() - 1) {
      file << " - ";
    }
  }
}


/**
 * @brief Additional method to show an iteration
 * @param int type interation
 * @param vector generated nodes
 * @param vector inspected nodes
 * @param ofstream name of the file to be written in
 */
void FirstSearchs::ShowIteration(unsigned iteration, const std::vector<unsigned>& generated_nodes, const std::vector<unsigned>& inspected_nodes, std::ofstream& file) {
  file << "Iteration " << iteration << std::endl;
  file << "Generated nodes: ";
  if (generated_nodes.empty()) {
    file << "-"; 
  } else {
    for (long unsigned int i = 0; i < generated_nodes.size(); ++i) {
      file << generated_nodes[i];
      if (i < generated_nodes.size() - 1) {
        file << ", ";
      }
    }
  }

  file << std::endl;

  file << "Inspected nodes: ";
  if (inspected_nodes.empty()) {
    file << "-"; 
  } else {
    for (long unsigned int i = 0; i < inspected_nodes.size(); ++i) {
      file << inspected_nodes[i];
      if (i < inspected_nodes.size() - 1) {
        file << ", ";
      }
    }
  }

  file << std::endl;
  file << "_______________________________________________________________________________________________________________________________________________" << std::endl;
}

