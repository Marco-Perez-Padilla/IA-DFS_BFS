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

** Archivo first_searchs.h: Declaracion de la clase FirstSearchs para manejar los recorridos en un grafo
**      
** Referencias:
**      Enlaces de interes

** Historial de revisiones:
**      27/09/2024 - Creacion del código
**      15/01/2025 - Arreglo de las funciones del BFS
**      31/05/2025 - Arreglo de DFS y refactorizacion del codigo
**/

#ifndef FISRT_SEARCHS_H
#define FIRST_SEARCHS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <limits>
#include <vector>
#include <queue>
#include <set>

struct ListNode {
	unsigned node; 
	long unsigned cost; 
  std::vector<ListNode> predecesors;
};

typedef std::vector<ListNode> AdyacentListNode;  

class FirstSearchs {
 private:
  unsigned number_nodes_;                      
  std::vector<AdyacentListNode> adyacent_list_;     

  void destroy();               
  void build (char namefile[85], int &error_flag); 

  void RecursiveDFS(unsigned current_node, unsigned destiny, std::vector<AdyacentListNode>& list, std::vector<unsigned>& path,
    std::vector<unsigned>& sucesors, std::vector<unsigned>& distances,
    std::vector<unsigned>& generated_nodes, std::vector<unsigned>& inspected_nodes, unsigned& iteration, bool& found, std::ofstream& file);

  void RecursiveBFS(unsigned origin_node, unsigned destiny, unsigned current_node, std::queue<unsigned>& queue, 
    std::vector<bool>& visited, std::vector<unsigned>& sucesors, std::vector<unsigned>& distance, std::vector<AdyacentListNode>& list, 
    std::vector<unsigned>& generated_nodes, std::vector<unsigned>& inspected_nodes, bool& found, unsigned& iteration, 
    std::vector<std::vector<std::pair<std::set<unsigned>, std::vector<unsigned>>>>& levels, std::vector<unsigned>& path, std::ofstream& file);
  
 public:
  FirstSearchs(char namefile[], int &error_flag);
  ~FirstSearchs(); 
  void update (char namefile[], int &error_flag);

  void ShowList(const std::vector<AdyacentListNode>& list);  
  void ShowLists ();

  void DepthSearch(std::ofstream& file);
  void BreadthSearch(std::ofstream& file); 
  void ShowIteration(unsigned iteration, const std::vector<unsigned>& generated_nodes, const std::vector<unsigned>& inspected_nodes, std::ofstream& file);
};

void pressanykey();
void clrscr();
void ShowPathDFS(unsigned s, unsigned i, std::vector<unsigned> predecesors, std::ofstream& file);
void ShowPathBFS(const std::vector<unsigned>& path, std::ofstream& file);

#endif