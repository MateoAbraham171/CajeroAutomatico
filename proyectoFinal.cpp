#include <iostream>
#include <sstream> //proporciona la clase std::stringstream que permite trabajar con flujos de caracteres en memoria//
#include <fstream> //proporciona las clases y funciones necesarias para trabajar con archivos en C++.//
#include <iomanip> //es para el setprecision//
using namespace std;

//definimos el nombre del archivo para trabajar mas comodamente///
#define NOMBRE_ARCHIVO "cuentasBancarias.txt" 

//trabajaremos con la siguiente estructura de usuario que almacena los datos necesarios para llevar a cabo las funciones del cajero//
struct usuario
{
    string nombre;
    string nroCuenta;
    string pin;
    long double balance;
    bool cuenta_bloqueada;
};

void cargarArchivo(usuario arr[], int& maximo_usuarios, string& encabezado);
int ingresoUsuario(usuario arr[], int maximo_usuarios);
void crearCuenta(usuario arr[], int tope_usuarios);
int menu(usuario arr[], int posicion);
void cerrarArchivo();
void mostrarBalance(long double Balance);
void depositarDinero(long double& Balance);
void retirarDinero(long double& Balance);
void transferirDinero (usuario arreglo[], int maximo_usuarios, long double& Balance);
void sobreescribirArchivo (usuario arreglo[], string encabezado, int maximo_cuentas);


int main()
{
    //nuestro archivo tendra un tope de 10 usuarios//
    int MAX_USUARIOS = 10;
    usuario aCuentas[MAX_USUARIOS];
    string Encabezado;
    int cuentas_existentes = 0;
    bool salir = false;

    //en la siguiente funcion abriremos el archivo y copiaremos su informacion en el arreglo creado previamente//
    cargarArchivo(aCuentas, cuentas_existentes, Encabezado);
    //en la siguiente funcion se busca si existe la cuenta y la posicion de la misma//
    int pos = ingresoUsuario(aCuentas, cuentas_existentes);
    //si la cuenta no ha sido bloqueada, entramos al menu//
    if (pos != -1)
    { 
        do
        {
            //ejecutamos la funcion del menu que nos devuelve la opcion que eligio el usuario//
            int opcion = menu(aCuentas, pos);
            //dependiendo la opcion que haya elegido el usuario, es el case que se ejecutara//
            switch (opcion)
            {
                case 1: //mostrar balance
                {
                    mostrarBalance(aCuentas[pos].balance);
                } 
                break;
                case 2:  //depositar dinero
                {
                    depositarDinero(aCuentas[pos].balance);
                }
                break;
                case 3:  //retirar dinero.
                {
                    retirarDinero(aCuentas[pos].balance);
                }
                break;
                case 4:  //transferir dinero
                {
                    transferirDinero(aCuentas, MAX_USUARIOS, aCuentas[pos].balance);
                }
                break;
                case 5:  //salir
                {
                    sobreescribirArchivo(aCuentas, Encabezado, cuentas_existentes);
                    salir = true;
                }
                break;
                default:
                {
                    cout << "Opcion no validad. Intente nuevamente." << endl;
                }
                break;
            }
        }while(!salir);
    }
    else
    {
        cout << endl << "Lo sentimos, se ha bloqueado su cuenta :( Dirijase al banco mas cercano.";
        sobreescribirArchivo(aCuentas, Encabezado, cuentas_existentes);
    }

    return 0;
}

void cargarArchivo(usuario arr[], int& maximo_usuarios, string& encabezado)
{
    //abrimos el archivo//
    ifstream archivoEntrada(NOMBRE_ARCHIVO); 
    string linea, str_nombre, str_nroCuenta, str_pin, str_balance, str_bloqueada;
    char delimitador = ',';
    int i = 0;

    //si el archivo está abierto se ejecuta 
    if(archivoEntrada.is_open())
    {
        //leemos la primer linea para descartarla (es el encabezado)//
        getline(archivoEntrada, linea);
        encabezado = linea;
        //leemos el resto de lineas y las guardamos en un arreglo//
        while (getline(archivoEntrada, linea))
        {
            //convertimos la cadena a stream//
            stringstream stream(linea);
            //leemos y guardamos los valores//
            getline(stream, str_nombre, delimitador);
            getline(stream, str_nroCuenta, delimitador);
            getline(stream, str_pin, delimitador);
            getline(stream, str_balance, delimitador);
            getline(stream, str_bloqueada, delimitador);

            //ingresamos los datos en el arreglo//
            arr[i].nombre = str_nombre;
            arr[i].nroCuenta = str_nroCuenta;
            arr[i].pin = str_pin;
            
            //pasamos el balance a long double y el booleano de cuenta bloqueada//
            if(str_bloqueada == "1")
            {
                arr[i].cuenta_bloqueada = true;
            }
            else if(str_bloqueada == "0")
            {
                arr[i].cuenta_bloqueada = false;
            }
            else
            {
                cout << "Error, faltan datos en el archivo." << endl;
            }

            arr[i].balance = stold(str_balance);

            //i nos servira para saber cuantas cuentas existen//
            i++;
        }
        //guardamos la cantidad de cuentas que existen en el archivo//
        maximo_usuarios = i;
        //cerramos el archivo hasta volver a necesitarlo//
        archivoEntrada.close();
    }
    else
    {
        cout << "Error al abrir el archivo." << endl;
    }
}

int ingresoUsuario(usuario arr[], int maximo_usuarios)
{
    bool nroCuentaOK = false;
    bool pinCorrecto = false;
    bool cuentaBloqueada = false;
    string numero_ingresado, pin_ingresado;
    //creamos una variable i para almacenar la posicion de la cuenta//
    int i;
    int nro_intentos = 3;
    int intentosPIN = 3;
    //mostramos un cartel de bienvenida
    cout << "**************Bienvenido a Cajeros Automaticos BB**************\n" << endl;
    while(nro_intentos != -1 && !pinCorrecto)
    {
        i = 0;
        //mostramos un mensaje para que el usuario ingrese el numero de cuenta//
        cout << "Ingrese su numero de cuenta: ";
        cin >> numero_ingresado;
        //mientras que no se haya encontrado la cuenta e i sea menor al maximo de usuarios, se repite//
        while (!nroCuentaOK && i < maximo_usuarios)
        {
            //si el dato ingresado coincide con algun nro de cuenta, salimos del bucle//
            if (arr[i].nroCuenta == numero_ingresado)
            {
                nroCuentaOK = true;
            } 
            else
            {
                i++;
            }
        }
        //si se encontro la cuenta de banco, pedimos el pin de seguridad//
        if(nroCuentaOK)
        {
            if (arr[i].cuenta_bloqueada == false)
            {
                //creamos un bucle que se repite mientras el pin no sea igual, tenga intentos y la cuenta no este bloqueada//
                while(!pinCorrecto && nro_intentos != -1)
                {
                    if(intentosPIN == 0)
                    {
                        arr[i].cuenta_bloqueada = true;
                        nro_intentos = -1;
                        i = nro_intentos;
                        pinCorrecto = true;
                    }
                    else if(intentosPIN == 1) 
                    {
                        cout << "\nTiene 1 intento.\n"
                        "Ingrese su PIN de seguridad: ";
                        cin >> pin_ingresado;
                        //restamos un intento//
                        intentosPIN--;
                    }
                    else
                    {
                        cout << "\nTiene " << intentosPIN << " intentos.\n"
                        "Ingrese su PIN de seguridad: ";
                        cin >> pin_ingresado;
                        //restamos un intento//
                        intentosPIN--;
                    }
                    //corroboramos si el pin ingresado coincide con el de la cuenta//
                    if (pin_ingresado == arr[i].pin)
                    {
                        pinCorrecto = true;
                    }
                    //si no coincide, !pinCorrecto seguira siendo false;
                }
            }
            else
            {
                nro_intentos = -1;
                i = nro_intentos;
            }
        }
        else
        {
            cout << "\nCuenta no encontrada. Intente nuevamente." << endl;
        }
    }
    //si i = -1, la cuenta se bloquea, en cambio, devolvemos el lugar donde se encuentra el usuario//
    return i;
}

int menu(usuario arr[], int posicion)
{
    int opcion;
    //creamos un bucle que se repite hasta que el usuario ingrese una opcion valida//
    do
    {
        cout << "\nBienvenido " << arr[posicion].nombre << "!!" << endl;
        cout << "\n********************************" << endl;
        cout << "--------MENÚ DE OPCIONES--------" << endl;
        cout << "********************************" << endl;
        cout << "1. Mostrar balance de cuenta .1" << endl;
        cout << "2. Depositar dinero .2" << endl;
        cout << "3. Retirar dinero .3" << endl;
        cout << "4. Transferir dinero .4" << endl;
        cout << "5. Cerrar sesion y salir .5\n" << endl; 
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if ((opcion <= 0) || (opcion >=6))
        {
            cout << "\nIngrese una opcion válida, por favor." << endl;
        }
    }
    while((opcion <= 0) || (opcion >=6));
    //devolvemos la opcion que haya ingresado el usuario//
    return opcion;
}

void mostrarBalance(long double Balance)
{
    //usamos el modificador FIXED para mostrar el numero completo y la funcion SETPRECISION para controlar la muestra de decimales//
    cout << "\nEl balance de su cuenta es: $" << fixed << setprecision(1) << Balance << ".\n\n";
}

void depositarDinero(long double& Balance)
{
    long double deposito;
    cout << "\nIngrese el monto a depositar (ingrese un punto si desea agregar decimales): ";
    cin >> deposito;

    Balance += deposito;
    cout << "Deposito exitoso! Su balance es de $" << fixed << setprecision(1) << Balance << ".\n\n";
}

void retirarDinero(long double& Balance)
{
    long double retiro;
    bool retiroOK = false;

    do
    {
        cout << "\nIngrese la cantidad que desea retirar (ingrese un punto si desea agregar decimales):";
        cin >> retiro;

        if (retiro > Balance)
        {
            cout << "\nNo posee dicha cantidad en su cuenta. Intente nuevamente.\n";
        }
        else
        {
            Balance -= retiro;
            retiroOK = true;

            cout << "\n\nRetiro exitoso! Su balance es de $" << fixed << setprecision(1) << Balance << ".\n\n";
        }
    }while(!retiroOK);
}

void transferirDinero (usuario arreglo[], int maximo_usuarios, long double& Balance)
{
    string cuentaARecibir; 
    long double monto;
    bool cuentaEncontrada = false;
    bool transferenciaExitosa = false;
    int i = 0;

    do
    {
        cout << "\nIngrese el numero de cuenta al que desea transferir: ";
        cin >> cuentaARecibir;

        while(!cuentaEncontrada && i < maximo_usuarios)
        {
            //buscamos si la cuenta existe en el arreglo//
            if (arreglo[i].nroCuenta == cuentaARecibir)  
            {
                cuentaEncontrada = true;
            }
            else
            {
                i++;
            }
        }
        //si la cuenta existe, comenzamos la transferencia//
        if(cuentaEncontrada)
        {
            while(!transferenciaExitosa)
            {
                cout << "\nIngrese el monto que desea transferir (ingrese un punto para los decimales): ";
                cin >> monto;

                //corroboramos que el monto no sea mayor al balance
                if (monto > Balance)
                {
                    cout << "\nNo posee esa cantidad en su cuenta. Intente nuevamente.\n";
                }
                else
                {
                    //si todo esta en orden, se realiza la transferencia//
                    //primero restamos el monto al balance y guardamos el resultado de la resta en el balance//
                    Balance -= monto;
                    //luego sumamos el valor del monto al balance de la cuenta a transferir//
                    arreglo[i].balance += monto;
                    cout << "\nTransferencia exitosa!\n";
                    //modificamos el valor bool de la siguiente variable para salir de los bucles//
                    transferenciaExitosa = true;
                }
            }
        }
        else
        {
            cout << "\nCuenta no encontrada. Intente nuevamente." << endl;
        }
    }while(!transferenciaExitosa);
}

void sobreescribirArchivo (usuario arreglo[], string encabezado, int maximo_usuarios)
{
    //abrimos el archivo//
    ofstream archivoSalida(NOMBRE_ARCHIVO);

    if(archivoSalida.is_open())
    {
        //primero copiamos el encabezado//
        archivoSalida << encabezado << endl;
        //luego copiamos todo el arreglo//
        for(int i = 0; i < maximo_usuarios; i++)
        {
            archivoSalida << arreglo[i].nombre << ","
            << arreglo[i].nroCuenta << ","
            << arreglo[i].pin << ","
            << arreglo[i].balance << ","
            << arreglo[i].cuenta_bloqueada << "," << endl;
        }
        //cerramos el archivo//
        archivoSalida.close();
    } 
    else
    {
        cout << "Error al abrir el archivo." << endl;
    }
}
