#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <iomanip>
using namespace std;

struct Paciente {
    string nombre;
    int edad;
    float altura;
    float peso;
    string tipoSangre;
    string sexo;
    char tieneEnfermedad;
    string enfermedad;
};

vector<Paciente> pacientes;

void limpiarPantalla() {
    system("cls || clear");
}

void pausa() {
    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void guardarPacientes() {
    if (pacientes.empty()) {
        cout << "No hay pacientes para guardar" << endl;
        return;
    }

    string nombreArchivo;
    cout << "Ingrese el nombre del archivo para guardar (sin extension): ";
    getline(cin >> ws, nombreArchivo);

    // Guardar en archivo binario
    string nombreBin = nombreArchivo + ".dat";
    ofstream archivoBin(nombreBin, ios::binary);
    if (!archivoBin) {
        cout << "Error al crear el archivo binario" << endl;
        return;
    }

    // Guardar en archivo de texto
    string nombreTxt = nombreArchivo + ".txt";
    ofstream archivoTxt(nombreTxt);
    if (!archivoTxt) {
        cout << "Error al crear el archivo de texto" << endl;
        return;
    }

    // Guardar datos binarios
    size_t numPacientes = pacientes.size();
    archivoBin.write(reinterpret_cast<const char*>(&numPacientes), sizeof(numPacientes));

    // Escribir encabezado en archivo de texto
    archivoTxt << "LISTA DE PACIENTES\n";
    archivoTxt << "===================\n\n";

    for (const auto& p : pacientes) {
        // Guardar en binario
        size_t size = p.nombre.size();
        archivoBin.write(reinterpret_cast<const char*>(&size), sizeof(size));
        archivoBin.write(p.nombre.c_str(), size);

        archivoBin.write(reinterpret_cast<const char*>(&p.edad), sizeof(p.edad));
        archivoBin.write(reinterpret_cast<const char*>(&p.altura), sizeof(p.altura));
        archivoBin.write(reinterpret_cast<const char*>(&p.peso), sizeof(p.peso));

        size = p.tipoSangre.size();
        archivoBin.write(reinterpret_cast<const char*>(&size), sizeof(size));
        archivoBin.write(p.tipoSangre.c_str(), size);

        size = p.sexo.size();
        archivoBin.write(reinterpret_cast<const char*>(&size), sizeof(size));
        archivoBin.write(p.sexo.c_str(), size);

        archivoBin.write(&p.tieneEnfermedad, sizeof(p.tieneEnfermedad));

        size = p.enfermedad.size();
        archivoBin.write(reinterpret_cast<const char*>(&size), sizeof(size));
        archivoBin.write(p.enfermedad.c_str(), size);

        // Guardar en texto
        archivoTxt << "PACIENTE\n";
        archivoTxt << "--------\n";
        archivoTxt << "Nombre: " << p.nombre << "\n";
        archivoTxt << "Edad: " << p.edad << " años\n";
        archivoTxt << "Altura: " << fixed << setprecision(2) << p.altura << " m\n";
        archivoTxt << "Peso: " << p.peso << " kg\n";
        archivoTxt << "Tipo de sangre: " << p.tipoSangre << "\n";
        archivoTxt << "Sexo: " << p.sexo << "\n";
        archivoTxt << "Enfermedad: " << (p.tieneEnfermedad == 'S' ? "Si" : "No") << "\n";
        if (p.tieneEnfermedad == 'S') {
            archivoTxt << "Detalle: " << p.enfermedad << "\n";
        }
        archivoTxt << "\n";
    }

    cout << "Datos guardados en " << nombreBin << " (binario) y " << nombreTxt << " (texto)" << endl;
}

void cargarPacientes() {
    string nombreArchivo;
    cout << "Ingrese el nombre del archivo a cargar (sin extension): ";
    getline(cin >> ws, nombreArchivo);
    nombreArchivo += ".dat";

    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) {
        cout << "No se pudo abrir el archivo " << nombreArchivo << endl;
        pausa();
        return;
    }

    pacientes.clear();
    size_t numPacientes;
    archivo.read(reinterpret_cast<char*>(&numPacientes), sizeof(numPacientes));

    cout << "\n=== PACIENTES CARGADOS ===\n";

    for (size_t i = 0; i < numPacientes; ++i) {
        Paciente p;
        size_t size;

        archivo.read(reinterpret_cast<char*>(&size), sizeof(size));
        p.nombre.resize(size);
        archivo.read(&p.nombre[0], size);

        archivo.read(reinterpret_cast<char*>(&p.edad), sizeof(p.edad));
        archivo.read(reinterpret_cast<char*>(&p.altura), sizeof(p.altura));
        archivo.read(reinterpret_cast<char*>(&p.peso), sizeof(p.peso));

        archivo.read(reinterpret_cast<char*>(&size), sizeof(size));
        p.tipoSangre.resize(size);
        archivo.read(&p.tipoSangre[0], size);

        archivo.read(reinterpret_cast<char*>(&size), sizeof(size));
        p.sexo.resize(size);
        archivo.read(&p.sexo[0], size);

        archivo.read(&p.tieneEnfermedad, sizeof(p.tieneEnfermedad));

        archivo.read(reinterpret_cast<char*>(&size), sizeof(size));
        p.enfermedad.resize(size);
        archivo.read(&p.enfermedad[0], size);

        pacientes.push_back(p);

        // Mostrar datos del paciente cargado
        cout << "\nPACIENTE #" << (i+1) << ":\n";
        cout << "Nombre: " << p.nombre << "\n";
        cout << "Edad: " << p.edad << " años\n";
        cout << "Altura: " << fixed << setprecision(2) << p.altura << " m\n";
        cout << "Peso: " << p.peso << " kg\n";
        cout << "Tipo de sangre: " << p.tipoSangre << "\n";
        cout << "Sexo: " << p.sexo << "\n";
        cout << "Enfermedad: " << (p.tieneEnfermedad == 'S' ? "Si" : "No") << "\n";
        if (p.tieneEnfermedad == 'S') {
            cout << "Detalle: " << p.enfermedad << "\n";
        }
    }

    cout << "\nTotal de pacientes cargados: " << numPacientes << endl;
    pausa();
}

void registrarPaciente() {
    Paciente p;
    limpiarPantalla();
    cout << "REGISTRO DE PACIENTE" << endl;
    cout << "Nombre: ";
    getline(cin >> ws, p.nombre);

    cout << "Edad: ";
    while (!(cin >> p.edad)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Edad invalida Ingrese un numero: ";
    }

    cout << "Altura (m): ";
    while (!(cin >> p.altura) || p.altura <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Altura invalida Ingrese un numero mayor a 0: ";
    }

    cout << "Peso (kg): ";
    while (!(cin >> p.peso) || p.peso <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Peso invalido Ingrese un numero mayor a 0: ";
    }

    cout << "Tipo de sangre: ";
    cin >> ws;
    getline(cin, p.tipoSangre);

    cout << "Sexo: ";
    cin >> ws;
    getline(cin, p.sexo);

    char respuesta;
    cout << "Alguna enfermedad a tener en cuenta (S/N): ";
    cin >> respuesta;
    p.tieneEnfermedad = (respuesta == 's' || respuesta == 'S') ? 'S' : 'N';
    p.enfermedad = "";

    if (p.tieneEnfermedad == 'S') {
        cout << "Cual es su enfermedad o condicion: ";
        getline(cin >> ws, p.enfermedad);
    }

    pacientes.push_back(p);
    cout << "Paciente registrado exitosamente" << endl;
    pausa();
}

void mostrarPacientes() {
    limpiarPantalla();
    cout << "LISTA DE PACIENTES" << endl;
    if (pacientes.empty()) {
        cout << "No hay pacientes registrados" << endl;
    } else {
        for (size_t i = 0; i < pacientes.size(); ++i) {
            cout << "\nPaciente #" << i + 1 << endl;
            cout << "Nombre: " << pacientes[i].nombre << endl;
            cout << "Edad: " << pacientes[i].edad << " años" << endl;
            cout << "Altura: " << fixed << setprecision(2) << pacientes[i].altura << " m" << endl;
            cout << "Peso: " << pacientes[i].peso << " kg" << endl;
            cout << "Tipo de sangre: " << pacientes[i].tipoSangre << endl;
            cout << "Sexo: " << pacientes[i].sexo << endl;
            cout << "Enfermedad: " << (pacientes[i].tieneEnfermedad == 'S' ? "Si" : "No") << endl;
            if (pacientes[i].tieneEnfermedad == 'S') {
                cout << "Detalle: " << pacientes[i].enfermedad << endl;
            }
        }
    }
    pausa();
}

void consultaMedica() {
    limpiarPantalla();
    cout << "CONSULTA MEDICA" << endl;
    cout << "Seleccione su condicion actual:" << endl;
    cout << "1 Dolor de cabeza" << endl;
    cout << "2 Fiebre" << endl;
    cout << "3 Dolor de estomago" << endl;
    cout << "4 Dolor muscular" << endl;
    cout << "5 Me siento bien" << endl;
    cout << "Opcion: ";

    int opcion;
    cin >> opcion;

    switch (opcion) {
        case 1:
        case 2:
        case 3:
        case 4:
            cout << "\nSe le ha recetado medicamento para su condicion" << endl;
            cout << "Descanse y tome liquidos" << endl;
            break;

        case 5:
            cout << "\nExcelente que se sienta bien" << endl;
            cout << "Mantenga habitos saludables" << endl;
            break;

        default:
            cout << "\nOpcion no valida" << endl;
    }

    pausa();
}

int main() {
    string usuario = "admin";
    string contrasena = "1234";
    string userInput, passInput;
    int opcion;

    limpiarPantalla();
    cout << "SISTEMA HOSPITALARIO" << endl;

    cout << "Usuario: ";
    getline(cin, userInput);
    cout << "Contrasena: ";
    getline(cin, passInput);

    if (userInput != usuario || passInput != contrasena) {
        cout << "\nCredenciales incorrectas" << endl;
        return 1;
    }

    do {
        limpiarPantalla();
        cout << "MENU PRINCIPAL" << endl;
        cout << "1 Realizar consulta medica" << endl;
        cout << "2 Registrar paciente" << endl;
        cout << "3 Mostrar pacientes" << endl;
        cout << "4 Guardar pacientes" << endl;
        cout << "5 Cargar pacientes" << endl;
        cout << "6 Salir" << endl;
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = 0;
        }

        switch (opcion) {
            case 1:
                consultaMedica();
                break;

            case 2:
                registrarPaciente();
                break;

            case 3:
                mostrarPacientes();
                break;

            case 4:
                guardarPacientes();
                pausa();
                break;

            case 5:
                cargarPacientes();
                break;

            case 6:
                cout << "\nSaliendo del sistema..." << endl;
                break;

            default:
                cout << "\nOpcion no valida" << endl;
                pausa();
        }

    } while (opcion != 6);

    return 0;
}