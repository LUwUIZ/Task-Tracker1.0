#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "include/json.hpp"

using namespace std;
using json = nlohmann::json;

json jArray;

//Obtiene la fecha y la hora actuales
string currentDateTime(){

	time_t t = time(0);
	tm* now = localtime(&t);
	
	char buffer[128];
	strftime(buffer, sizeof(buffer), "%d-%m-%y %X", now);
	return buffer;
	
}

//Verifica que el documento de "Datos.txt" este creado al inicio del codigo y si no es asi lo crea
void fileCreator(){
	
	ifstream archivo("Datos.json", ios::in); //constructor
 
	//ios::in es para lectura

	if (!archivo.is_open()){ //verifica que el archivo exista
    	cout << "'datos.json' no existe. Se creará un nuevo archivo.\n";
        jArray = json::array(); // Inicializar el array JSON
	}
	
}

//Modifica el archivo de "Datos.json"
void fileModificator(string str, int estado){
	string sestado = "";
    int idCounter = jArray.size() + 1;
	
	if (estado){
		sestado = "ACTIVO" ;
	}else{
		sestado = "INACTIVO" ;
	}
	
	json j;
		
    j["id"] = idCounter;
    j["registro"] = str;
    j["estado"] = sestado;
    j["fecha"] = currentDateTime();     
    
    cin.ignore();
    
    jArray.push_back(j);
    idCounter++;
    
    ofstream archivo("datos.json");
    archivo.seekp(0, ios::end); //Se mueve hasta el final del documento
    
    
    if (archivo.is_open()) {
        archivo << jArray.dump(2);
        archivo.close(); // Cerrar el archivo
        cout << "Los datos se han guardado en 'datos.json'.\n";
    } else {
        cerr << "Error al abrir el archivo.\n";
    }
    
    system ("cls");
	
}

void fileReader(){
	
	ifstream archivo("datos.json");
	
	if (!archivo.is_open()) {
        cerr << "No existen Registros." << endl;
    }
    
    archivo >> jArray;
	printf ("Datos cargados desde 'datos.json': \n");
    cout << jArray.dump(2) << endl; 
	
}

void buscarPorID(const json& jArray) {
	int id = 0;
	
	printf("Ingresa el ID que deseas buscar: \n");
	scanf("%d", &id);
	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	
    for (const auto& item : jArray) { 
        if (item["id"] == id) {
            cout << "Registro encontrado:\n";
            cout << item.dump(2) << "\n"; // Imprimir el registro encontrado
            
            ofstream archivoSalida("datos.json");
    		if (archivoSalida.is_open()) {
        		archivoSalida << jArray.dump(2); 
        		archivoSalida.close(); // Cerrar el archivo
        		cout << "Los cambios se han guardado en 'datos.json'.\n";
    		} else {
        		cerr << "Error al abrir el archivo para guardar los cambios.\n";
    		}
            return;
        }
    }
    cout << "No se encontró un registro con ID " << id << ".\n";
}

void guardarArchivo(json& jArray){
	
	ofstream archivoSalida("datos.json");
    if (archivoSalida.is_open()) {
        archivoSalida << jArray.dump(2); // Escribir el JSON con 4 espacios de indentación
    	archivoSalida.close(); // Cerrar el archivo
		cout << "Los cambios se han guardado en 'datos.json'.\n";
    }else{
    	cerr << "Error al abrir el archivo para guardar los cambios.\n";
    	return;
    }
    
}

void borrarRegistro(json& jArray){
		
	int id = 0;
	int idel =0;
	
	printf("Ingresa el ID que deseas borrar: \n");
	scanf("%d", &id);
	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	
	for (const auto& item : jArray) { 
        if (item["id"] == id) {
            cout << "Registro encontrado: \n";
            cout << item.dump(2) << "\n"; // Imprimir el registro encontrado
            cout << "Esta seguro de preoceder con el borrado ? \n"
            		"[0].......NO \n"
            		"[1].......SI \n";
            scanf("%d", &idel);
            
            if (idel) {
				jArray.erase(jArray.begin() + id - 1); // Borrar el registro
				ofstream archivoSalida("datos.json");
				guardarArchivo(jArray);
    			
				return; // Salir después de borrar
			}else{
				cout << "Se cancelo el borrado";
				return;
			}
        }
    }
    
    cout << "No se encontró un registro con ID " << id << ".\n";

}

void actualizarRegistro(json& jArray, int id) {
	string nuevoNombre = "";
	
	printf ("Ingrese el nuevo nombre: \n");
    getline(cin, nuevoNombre);
	
    // Encontrar y actualizar el nombre del registro
    for (auto& elemento : jArray) {
        if (elemento["id"] == id) {
            elemento["registro"] = nuevoNombre; // Actualizar el nombre
            cout << "Nombre actualizado a " << nuevoNombre << " para ID " << id << ".\n";
            guardarArchivo(jArray);
            
            return; // Salir después de actualizar
        }
    }
    cout << "No se encontró un registro con ID " << id << ".\n";
}

int main() {
		
	int iopc = 0, id = 0, iestado = 0;
	bool out = true;
    string nuevoNombre = "", sactividad = "", sestado = "";
	
	ifstream archivoLeer("datos.json");
    archivoLeer >> jArray;
    
	fileCreator();
	printf ("Bienvenido al task tracker \n") ;
	while (true){
		
		printf ("Elige una Opcion \n");
		printf ("[1].........Agregar un registro \n");
		printf ("[2].........Eliminar un registro \n");
		printf ("[3].........Actualizar un registro \n");
		printf ("[4].........Cambiar el estado de un registro \n");
		printf ("[5].........Ver todos los registros \n");
		printf ("[6].........Buscar Registro por ID \n");
		printf ("[7].........Salir \n");
		scanf ("%d", &iopc);
		cin.ignore(numeric_limits<streamsize>::max(),'\n'); //Espera a que se presione enter para continuar
		system ("cls");

		switch (iopc){
			case 0: //en caso de que se ingresen letras lo reconoce como 0
				printf("La opcion ingresada es INVALIDA, Porfavor Introduce un numero \n" );
				cout << "Press Enter to Continue";
				cin.ignore(numeric_limits<streamsize>::max(),'\n');
				break;
			case 1:
				printf (" ***OPCION Agregar un Registro*** \n ");
				printf ("-----------------------------------\n ");
				printf("Introduce el registro: \n");
				getline(cin, sactividad);
				
				fileModificator(sactividad, iestado);
				
				break;
			case 2:
				printf (" ***OPCION Eliminar un Registro*** \n ");
				printf ("-----------------------------------\n ");
				
				borrarRegistro(jArray);
				
				break;
			case 3:
				printf (" ***OPCION Actualizar un Registro*** \n ");
				printf ("-----------------------------------\n ");
				
    			printf ("Ingrese el ID del registro a actualizar: \n");
				scanf("%d", &id);
				
				actualizarRegistro(jArray, id);
				
				break;
			case 4:
				while (out) {
					printf (" ***OPCION Cambiar el estado de un Registro*** \n ");
					printf ("-----------------------------------\n ");
					printf("Que estado deseas registrar ? \n");
					printf ("Elige una Opcion \n");
					printf ("[1].........ACTIVO \n");
					printf ("[2].........INACTIVO \n");
					printf ("[3].........FINALIZADO \n");
					scanf("%d", &iestado);
					system ("cls");
				
					if (iestado == 1){
						sestado = "ACTIVO";
						out = false;
					}else if (iestado == 2){
						sestado = "INACTIVO";
						out = false;
					}else if (iestado == 3){
						sestado = "FINALIZADO";
						out = false;
					}else{
						printf ("Escoge una opcion entre 1 - 3 \n");
						cout << "Press Enter to Continue";
						cin.ignore(numeric_limits<streamsize>::max(),'\n');
						system ("cls");
					}
				}
				system("cls");
				break;
			case 5:
				printf (" ***OPCION Ver todos los Registro*** \n ");
				printf ("-----------------------------------\n ");
				fileReader();
				cout << "Press Enter to Continue";
				cin.ignore(numeric_limits<streamsize>::max(),'\n');
				system("cls");
				break;
			case 6:
				printf (" ***OPCION Buscar registro por ID*** \n ");
				printf ("-----------------------------------\n ");
				buscarPorID(jArray);
				break;
			case 7:
				printf("Gracias por utilzar Task Tracker ;p \n");
				return 0;
				break;
			default:
				printf("Opcion no valida, porfavor de escoger un numero entre 1 y 5 \n");
				break;
		}
	}
}
