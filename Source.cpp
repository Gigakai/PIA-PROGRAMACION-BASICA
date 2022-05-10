#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;
int available_empaste{ 10 }, available_blanqueo{ 10 }, available_ajuste{ 10 }, available_endodoncia{ 10 };

int identificar_disponibilidad(string selected_trat);
struct Cita {
	int num_cita{};
	string nombre;
	string hora_cita{};
	string name_trat;
	string tratamiento{};
	string descripcion;
	float precio_uni{ 0 };
	string amount{};
	float subtotal{ 0 };
	float total{ 0 };
	Cita* siguiente{ nullptr };
	Cita* anterior{ nullptr };
	void mostrar_cita() {
		cout << "Nombre del paciente: " << nombre << endl;
		cout << "Hora de la cita: " << hora_cita << ":00" << endl;
		cout << "Tratamiento:" << name_trat << endl;
		cout << "Descripcion del tratamiento: " << descripcion << endl;
		cout << "Cantidad usada de tratamiento: " << amount << endl;
		cout << showpoint << setprecision(5);
		cout << "Precio por unidad: $" << precio_uni << endl;
		cout << "Subtotal: $" << subtotal << endl;
		cout << "Total: $" << total;
	}

	void calcular_subtotal() {
		subtotal = precio_uni * stof(amount);
	}

	void calcular_total() {
		total = subtotal + (subtotal * 0.16);
	}
	void validar_tratamiento_amount() {
		bool valid_trat{ false };
		do {
			system("cls");
			cout << "Los tratamientos disponibles son:\n12000 - Empaste---------Disponibles: " << available_empaste << "\n12001 - Blanqueo de dientes---------Disponibles: " << available_blanqueo << "\n12002 - Ajuste de brackets---------Disponibles: " << available_ajuste << "\n12003 - Endodoncia---------Disponibles: " << available_endodoncia << "\nIngrese aqui el codigo del tratamiento que elegira(Numero que se encunetra a la derecha del tratamiento) : ";
			getline(cin, tratamiento);
			if (tratamiento != "12000" && tratamiento != "12003" && tratamiento != "12002" && tratamiento != "12001") {
				cout << "Opcion invalida, vuelva a ingresarla\n";
				cin.get();
			}
			else {
				int cantidad_trat{};
				cantidad_trat = identificar_disponibilidad(tratamiento);
				if (cantidad_trat == 0) {
					cout << "Tratamiento no disponible, intentelo con otro" << endl;
					cin.get();
				}
				else {
					valid_trat = true;
				}
			}

		} while (!valid_trat);
	}
	void validar_amount() {
		bool valid_amount{ false };
		do {
			cout << "Ingrese la cantidad del tratamiento (ej. 9) [Max. 2 por persona]: ";
			getline(cin, amount);
			valid_amount = true;
			for (char c : amount) {
				if (isdigit(c) == 0) {
					valid_amount = false;
				}
			}
			if (valid_amount == true)
				if (stoi(amount) < 1 || stoi(amount) > 2) {
					valid_amount = false;
				}
			if (valid_amount == false) {
				cout << "Cantidad invalida, ingresela de nuevo " << endl;
			}
		} while (!valid_amount);
	}
	void validar_hora() {
		bool valid_hora{ false };
		do {
			bool numeric_hour{ false };
			cout << "Ingrese la hora en que comenzara el traramiento entre las 7:00 a 23:00 horas Formato 24h (ej. 08): ";
			getline(cin, hora_cita);
			for (char c : hora_cita) {
				if (isdigit(c) == false) {
					numeric_hour = true;
				}
			}
			if (numeric_hour == 0) {
				if (stoi(hora_cita) >= 7 && stoi(hora_cita) <= 23) {
					valid_hora = 1;
				}
				else {
					cout << "Hora invalida, ingresela de nuevo\n";
				}
			}
			else {
				cout << "Hora invalida, ingresela de nuevo\n";

			}
		} while (!valid_hora);

	}
	void asignar_preciouni() {
		const double blanqueo{ 112.50 }, empaste{ 250.50 }, ajuste{ 100 }, endodoncia{ 500 };
		switch (tratamiento.at(4)) {
		case '0': precio_uni = empaste; available_empaste -= stoi(amount);  name_trat = "Empaste"; break;
		case '1': precio_uni = blanqueo; available_blanqueo -= stoi(amount); name_trat = "Blanqueo"; break;
		case '2': precio_uni = ajuste; available_ajuste -= stoi(amount); name_trat = "Ajuste"; break;
		case '3': precio_uni = endodoncia; available_endodoncia -= stoi(amount); name_trat = "Endodoncia"; break;
		default:;
		}
	}
};


Cita* primer_cita{ nullptr };
Cita* ultima_cita{ nullptr };
Cita* encontrar_cita(Cita* auxiliar_cita);
bool repetir_accion();
void save_information();
void read_information();
int control_cita{ 1 };
void return_value(Cita* selected_cita);
void listar_citas();
int main() {
	string proceso_solicitado{};
	bool control{ false };
	const int codigo_blanqueo{ 12000 }, codigo_empaste{ 12001 }, codigo_ajuste{ 12002 }, codigo_endodoncia{ 12003 };
	read_information();
	cout << "---------------------------------------------------------------\nPresione enter para empezar a navegar en nuestra pagina\n---------------------------------------------------------------";
	while (!control) {
		cin.get();
		system("cls");
		Cita* selected_cita{ nullptr };
		cout << "---------------------Bienvienido a Alfa Dentista, esperamos que tenga una grata experiencia---------------------\nLas citas se manejan una por hora, desde las 7:00 AM hasta las 23:00PM\nOfrecemos blanqueo, endodoncia, empaste, y ajuste de brakets\nSeleccione una accion que quiera realizar, introduciendo el numero de la misma \n--------------------------------------------------------------- \n1-Agendar una cita\n2-Modificar cita\n3-Eliminar cita\n4-Lista de citas vigentes\n5-Limpiar pantalla\n6-Salir\n---------------------------------------------------------------" << endl;
		cout << "Ingrese el numero aqui (Ej. 1): ";
		cin >> proceso_solicitado;
		system("cls");
		switch (proceso_solicitado.at(0)) {
		case '1': {
			bool keep_adding{ false };
			while (!keep_adding) {
				Cita* nueva_cita{ new Cita };
				nueva_cita->num_cita = control_cita;
				cout << "---------------------Selecciono agendar una cita procedemos a agendarla---------------------" << endl;
				cout << "Su numero de cita es: " << nueva_cita->num_cita << endl;
				cout << "Ingrese el nombre completo de su paciente: ";
				cin.ignore();
				getline(cin, nueva_cita->nombre);
				nueva_cita->validar_hora();
				nueva_cita->validar_tratamiento_amount();
				nueva_cita->validar_amount();
				nueva_cita->asignar_preciouni();
				cout << "\nIngrese las especificaciones del tratamiento solicitado: ";;
				getline(cin, nueva_cita->descripcion);
				cout << "\nEl precio unitario es de: $" << nueva_cita->precio_uni << endl;
				nueva_cita->calcular_subtotal();
				cout << "\nEl subtotal es de: $" << nueva_cita->subtotal << endl;
				nueva_cita->calcular_total();
				cout << "\nEl total es de: $" << nueva_cita->total << endl;
				if (nueva_cita->num_cita == 1 || primer_cita == 0) {
					primer_cita = nueva_cita;
					ultima_cita = nueva_cita;
				}
				else {
					ultima_cita->siguiente = nueva_cita;
					ultima_cita->siguiente->anterior = ultima_cita;
					ultima_cita = nueva_cita;
				}
				control_cita++;
				keep_adding = repetir_accion();
			}
			break;
		}
		case '2': {
			if (primer_cita) {
				char modificar{};
				bool keep_modifying{ false };
				while (!keep_modifying) {
					cout << "---------------------Ha seleccionado modificar una cita---------------------" << endl;
					listar_citas();
					selected_cita = encontrar_cita(primer_cita);
					if (selected_cita) {
						cout << "Opciones para cambiar:\n---------------------------------------------------------------\n1- Nombre\n2-Tratamiento\n3-Descripcion\n4-Cantidad\n5-Hora\n6-Salir\n---------------------------------------------------------------" << endl;
						cout << "Ingrese aqui el numero de lo que se desea cambiar: ";
						cin >> modificar;
						switch (modificar) {
						case '1': {
							cout << "Escriba aqui el nuevo nombre: ";
							cin.ignore();
							getline(cin, selected_cita->nombre);
							break;
							break;
						}
						case '2': {
							return_value(selected_cita);
							cin.ignore();
							selected_cita->validar_tratamiento_amount();
							selected_cita->amount;
							selected_cita->asignar_preciouni();
							cout << "Escriba la nueva descripcion: ";
							getline(cin, selected_cita->descripcion);
							selected_cita->calcular_subtotal();
							selected_cita->calcular_total();
							cout << "\nEl precio unitario ahora es: $" << selected_cita->precio_uni << "\nEl subtotal es: $" << selected_cita->subtotal << "\nEl total es: $" << selected_cita->total << endl;
							break;
						}
						case '3': {
							cout << "Escriba la nueva descripcion: ";
							cin.ignore();
							getline(cin, selected_cita->descripcion);
							break;
						}
						case '4': {
							return_value(selected_cita);
							cin.ignore();
							selected_cita->validar_amount();
							selected_cita->calcular_subtotal();
							cout << "\nEl nuevo subtotal es: $" << selected_cita->subtotal << endl;
							selected_cita->calcular_total();
							cout << "\nEl nuevo total es: $" << selected_cita->total << endl;
							break;
						}
						case '5': {
							cin.ignore();
							selected_cita->validar_hora();
							break;
						}
						case '6': keep_modifying = true; break;
						default: cout << "Opcion invalida" << endl;
						}
						if (modificar != '6')
							keep_modifying = repetir_accion();
					}
					else {
						keep_modifying = true;
					}
				}
			}
			else {
				cout << "No hay citas programadas aun, se devolvera al menu principal" << endl;
				cin.get();
			}

			break;
		}
		case '3': {
			if (primer_cita) {
				cout << "---------------------Ha seleccionado eliminar una cita---------------------" << endl;
				listar_citas();
				selected_cita = encontrar_cita(primer_cita);
				if (selected_cita) {
					if (selected_cita->anterior) {
						selected_cita->anterior->siguiente = selected_cita->siguiente;
					}
					if (selected_cita->siguiente) {
						selected_cita->siguiente->anterior = selected_cita->anterior;
					}
					if (selected_cita->num_cita == primer_cita->num_cita) {
						primer_cita = primer_cita->siguiente;
					}
					if (selected_cita->num_cita == ultima_cita->num_cita) {
						ultima_cita = ultima_cita->anterior;
					}
					return_value(selected_cita);
					delete selected_cita;
					cout << "Se elimino excitosamente la cita, se le devolvera al menu principal";
				}
				else {
					cout << "Regresando al menu principal\n";
				}
			}

			else {
				cout << "No hay citas que se puedan eliminar, se le devolvera al menu principal";
			}
			cin.get();
			break;
		}
		case '4': {
			bool ver_cita{ false };

			int k{};
			char consultar{};
			cout << "---------------------Ha seleccionado ver la lista de citas vigentes---------------------" << endl;
			if (primer_cita) {
				while (!ver_cita) {
					listar_citas();
					selected_cita = encontrar_cita(primer_cita);
					if (selected_cita) {
						cout << "---------------------------------------------------------------\n";
						selected_cita->mostrar_cita();
						cout << "\n---------------------------------------------------------------";
						cout << endl;
						ver_cita = repetir_accion();
					}
					else {
						ver_cita = true;
					}

				}
				break;
			}
			else {
				cout << "No hay citas por ver, se le devolvera al menu principal" << endl;
				ver_cita = true;
				cin.get();
			}

			break;
		}
		case '5': {

			system("cls");
			cout << "Pantalla limpiada, presione cualquier tecla para continuar" << endl;
			system("pause>null");
			break;
		}
		case '6': {
			control = true;
			cout << "Esperamos que su experiencia fue grata usando nuestros servicios. Nos vemos." << endl;
			save_information();
			break;
		}
		default: {
			cout << "Opcion Invalida, se le regresara al menu inicial" << endl;
			cin.get();
		}
		}
	}
	return 0;
}
Cita* encontrar_cita(Cita* auxiliar_cita) {
	string num_appointment{};
	bool num_is_digit{ false };
	do {
		cout << "Ingresa el numero de la cita en la que quieres realizar la accion (Ingrese 0 si quiere abandonar esta opcion): ";
		cin >> num_appointment;
		num_is_digit = true;
		for (char c : num_appointment) {
			if (isdigit(c) == 0) {
				num_is_digit = false;
			}
		}
		if (num_is_digit == 0) {
			cout << "Se ingreso un dato invalido intetelo de nuevo\n";
			system("pause>0");
		}
	} while (!num_is_digit);
	bool finding_appointment{ false };
	if (stoi(num_appointment) != 0) {
		while (!finding_appointment) {
			if (auxiliar_cita) {
				if (auxiliar_cita->num_cita == stoi(num_appointment)) {
					finding_appointment = 1;
				}
				else {
					auxiliar_cita = auxiliar_cita->siguiente;
				}
			}
			else {
				cout << "Cita no encontrada, esta cita ha sido elimindad o no fue registrada." << endl;
				finding_appointment = 1;
				cin.get();
			}
		}
	}
	else {
		auxiliar_cita = nullptr;
	}
	return auxiliar_cita;
};
void listar_citas() {
	cout << "Estas son las citas vigentes: \n---------------------------------------------------------------" << endl;
	Cita* available_appointment{ primer_cita };
	while (available_appointment) {
		cout << "Cita No." << available_appointment->num_cita << endl;
		available_appointment = available_appointment->siguiente;
	}
	cout << "\n---------------------------------------------------------------\n";
}
bool repetir_accion() {
	char control_repeat{};
	bool repeat{ false };
	cout << "\nDesea realizar esta accion de nuevo [S/N]: ";
	cin >> control_repeat;
	switch (control_repeat) {
	case 'S':
	case 's': system("cls"); break;
	case 'n':
	case 'N': repeat = true; break;
	default: cout << "Se le devolvera al menu principal por ingresar una opcion invalida\n"; repeat = true; break;
	}
	return repeat;
};
void return_value(Cita* selected_cita) {
	switch (selected_cita->tratamiento.at(4)) {
	case '0': available_empaste += stoi(selected_cita->amount); break;
	case '1': available_blanqueo += stoi(selected_cita->amount); break;
	case '2': available_ajuste += stoi(selected_cita->amount); break;
	case '3': available_endodoncia += stoi(selected_cita->amount); break;
	}
};
int identificar_disponibilidad(string selected_trat) {
	switch (selected_trat.at(4)) {
	case '0': return available_empaste; break;
	case '1': return available_blanqueo; break;
	case '2': return available_ajuste; break;
	case '3': return available_endodoncia; break;
	}
};
Cita* aux_cita{ nullptr };
void save_information() {
	ofstream myFile;
	myFile.open("Registro_citas.txt");
	aux_cita = primer_cita;
	while (aux_cita) {
		myFile << aux_cita->num_cita << endl;
		myFile << aux_cita->nombre << endl;
		myFile << aux_cita->hora_cita << endl;
		myFile << aux_cita->tratamiento << endl;
		myFile << aux_cita->amount << endl;
		myFile << aux_cita->descripcion << endl;
		myFile << aux_cita->subtotal << endl;
		myFile << aux_cita->total << endl;
		aux_cita = aux_cita->siguiente;
	}
	myFile.close();
};
void read_information() {
	ifstream read_me;

	read_me.open("Registro_citas.txt", ios::in);
	if (read_me.is_open()) {
		bool text_availability{ false };
		string there_is_text{};
		while (getline(read_me, there_is_text)) {
			Cita* cita_leida{ new Cita };
			cita_leida->num_cita = stoi(there_is_text);
			getline(read_me, cita_leida->nombre);
			getline(read_me, cita_leida->hora_cita);
			getline(read_me, cita_leida->tratamiento);
			getline(read_me, cita_leida->amount);
			getline(read_me, cita_leida->descripcion);
			cita_leida->asignar_preciouni();
			read_me >> cita_leida->subtotal;
			read_me >> cita_leida->total;
			if (control_cita == 1) {
				primer_cita = cita_leida;
				ultima_cita = cita_leida;
			}
			else
			{
				ultima_cita->siguiente = cita_leida;
				ultima_cita->siguiente->anterior = ultima_cita;
				ultima_cita = cita_leida;
			}
			getline(read_me, there_is_text);
			control_cita++;

		}
		if (ultima_cita) {
			control_cita = (ultima_cita->num_cita) + 1;
		}
		read_me.close();
	}
};
