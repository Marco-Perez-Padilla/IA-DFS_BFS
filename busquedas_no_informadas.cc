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

** Archivo busquedas_no_informadas.cc: Programa cliente para procesar grafos y hacer busquedas no informadas
**      
** Referencias:
**      Enlaces de interes

** Historial de revisiones:
**      27/09/2024 - Creacion del código
**      15/01/2025 - Arreglo de las funciones del BFS
**      31/05/2025 - Arreglo de DFS y refactorizacion del codigo
**/

#include "first_searchs.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>
#include <string>

/**
 * @brief Start menu to choose an option of the following
 * @param char option to be used
 */
void menu (char &option) {
  std::cout << "c. [c]harge graph from file" << std::endl;     
  std::cout << "a. Show [a]dyacency list " << std::endl;
  std::cout << "b. Execute [b]readth first search algorithm" << std::endl;
  std::cout << "d. Execute [d]epth first search algorithm" << std::endl;          
  std::cout << "q. quit program" << std::endl;
  std::cout << "Introduce action to execute  > ";
  std::cin >> option;
};

void help () {
  std::cout << "How to use: ./busquedas_no_informadas <input> <output>" << std::endl;     
  std::cout << "<input>: text in format .txt specifyng a graph" << std::endl;
  std::cout << "<output> File to store search results" << std::endl;
}


int main(int argc, char *argv[]) {
  int error_flag;
  char namefile[85], outfile[85], option;
  

  // Procesamiento de línea de comandos
  if (argc == 3) {
	  std::cout << "Charging data" << std::endl;
    strcpy(namefile, argv[1]);  
    strcpy(outfile, argv[2]);
    clrscr();    
  } else if (argc == 2 && std::string(argv[1]) == "--help") {
    help();
    return 0;
  } else {
    std::cout << "Error in command line format. Use './busquedas_no_informadas --help' for further information" << std::endl;
    return 1;
  }

  // Creating graph
  FirstSearchs search_graph(namefile, error_flag);

  // Manage errors
  if (error_flag == 1) {
    std::cout << "Error opening the file: check route and extension" << std::endl;
    pressanykey();
    clrscr();
  } else {
    std::cout<< "Graph loaded from file " << namefile << std::endl;
    pressanykey();
    clrscr();
    
    do {
      menu(option);
      switch (option) {
        // Charge new graph
        case 'c' :
          clrscr();
          std::cout << "Introduce full route of the file (example: docs/Grafo1.txt)" << std::endl;
          std::cin >> namefile;
          search_graph.update(namefile, error_flag);

          if (error_flag == 1) {
            std::cout << "Error opening the file: check route and extension" << std::endl;
          } else {
            std::cout << "File successfully loaded from " << namefile << std::endl;
          }
          pressanykey();
          clrscr();
          break;

        // Show adyacency list
        case 'a' :
          clrscr();
          search_graph.ShowLists();
          pressanykey();
          clrscr();
          break;

        // Execute Breadth First Search
        case 'b' : {
          clrscr();
          std::ofstream out (outfile, std::ios::app);
          std::cout << "Breadth Search: " << std::endl;
          search_graph.BreadthSearch(out);
          std::cout << "File successfully generated" << std::endl;
          out.close();
          pressanykey();
          clrscr();
          break;
        }

        // Execute Depth First Search
        case 'd' : {
          clrscr();
          std::ofstream out (outfile, std::ios::app);
          std::cout << "Depth Search: " << std::endl;
          search_graph.DepthSearch(out);
          std::cout << "File successfully generated" << std::endl;
          out.close();
          pressanykey();
          clrscr();
          break;
        }
      }
    } while (option != 'q');
  }

  std::cout << "Finishing program" << std::endl;
	return 0;
};
