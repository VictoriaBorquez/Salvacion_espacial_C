/*Victoria Bórquez */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*Estructura Postulante
Contiene:
- nombre del postulante (tipo char*)
- peso del postulante (tipo int)
- calificación del postulante (tipo int)
*/
typedef struct Postulante{
  char * nombre;
  int peso;
  int calificacion;
} postulante;

/*Estructura Solucion
Contiene:
- nombres de postulantes (tipo char**)
- cantidad de postulantes que contiene la solucion (tipo int)
- suma de los pesos de los postulantes de la solucion (tipo int)
- suma de las calificaciones de los postulantes de la solucion (tipo int)
*/
typedef struct Solucion{
  char ** nombres;
  int cantidadNombres;
  int sumaPeso;
  int sumaCalificacion;
} solucion;

/*Función swap
Intercambia dos soluciones del arreglo de soluciones
Entrada: arreglo de soluciones, posiciones i,j a intercambiar
Salida: arreglo de soluciones con las posiciones intercambiadas
*/
solucion * swap(solucion * A, int i, int j){
  solucion aux;
  if(i!=j){
    aux = A[i];
    A[i] = A[j];
    A[j] = aux;
  }
  return A;
}
/* Algoritmo de ordenamiento - Método burbuja
Ordena los elementos de mayor a menor, en base a la calificacion total
Entrada: arreglo de estructura del tipo solución, largo del arreglo
Salida: arreglo ordenado de mayor a menor en base a calificaciones
*/
solucion * burbuja(solucion * A, int largo){
  int i,j;
  for(i = largo-1; i>=0; i--){
    for(j = 0; j<=i-1; j++){
      if(A[j].sumaCalificacion<A[j+1].sumaCalificacion){
        A = swap(A, j, j+1);
      }
    }
  }
  return A;
}

/*Funcion que escribe las soluciones en el archivo de salida
Entrada: arreglo de soluciones, cantidad de soluciones, archivo de salida
Salida: no aplica (void)
*/
void mostrarSoluciones(solucion * arr, int * cantSoluciones, FILE * f){
  int cantPersonas;
  int i,j;
  for(i = 0; i < *cantSoluciones; i++){
    cantPersonas = arr[i].cantidadNombres;
    for(j = 0; j < cantPersonas; j++){
      fprintf(f,"%s, ",arr[i].nombres[j]);
    }
    fprintf(f,"%d, ",arr[i].sumaPeso);
    fprintf(f,"%d\n",arr[i].sumaCalificacion);
  }
}

/*Funcion que agrega elementos al arreglo de soluciones
Entrada: arreglo original, cantidad de soluciones actual, solucion a agregar
Salida: arreglo con la solucion agregada
*/
solucion * agregarSolucion(solucion * arrOriginal, int * cantSoluciones, solucion nuevaSolucion){
  // se agrega el nuevo valor al arreglo
  arrOriginal[*cantSoluciones].nombres = nuevaSolucion.nombres;
  arrOriginal[*cantSoluciones].cantidadNombres = nuevaSolucion.cantidadNombres;
  arrOriginal[*cantSoluciones].sumaPeso = nuevaSolucion.sumaPeso;
  arrOriginal[*cantSoluciones].sumaCalificacion = nuevaSolucion.sumaCalificacion;
  // se modifica el valor de cantidad de soluciones
  *cantSoluciones = *cantSoluciones + 1;

  return arrOriginal;
}

/* Funcion que realiza las combinaciones posibles segun el archivo de entrada.
Y aplica la restricción segun el peso máximo que soporta la nave
Entrada: arreglo de soluciones, largo de las combinaciones a generar,
inicio (indice), nuevo arreglo de postulantes, largo del arreglo,
rConstante = cantidad de personas que postulan, cantidad de soluciones,
arreglo de soluciones final, peso maximo que soporta la nave
Salida: - (void ya que modifica directamente el arreglo de soluciones)
*/
void combinaciones(postulante * arreglo, int largo, int inicio, postulante * nuevoArreglo, int largoArreglo, int rConstante, int * cantidadSoluciones, solucion * solucionesFinal, int pesoMax){
  int sumaPesos;
  int sumaCalificaciones;
  int j,i;
  //caso base
  if(largo==0){
    sumaPesos = 0;
    sumaCalificaciones = 0;
    for(j = 0; j < rConstante; j++){
      sumaPesos = sumaPesos + nuevoArreglo[j].peso;
      sumaCalificaciones = sumaCalificaciones + nuevoArreglo[j].calificacion;
    }
    /*Si la suma de pesos de la solución no excede el peso máximo que soporta
    la nave, se crea una solucion con los datos del caso evaluado y se agrega
    al arreglo de soluciones*/
    if(sumaPesos <= pesoMax){
      char ** nombresSolucion = (char **)malloc(sizeof(char *)*rConstante);
      for(j = 0; j < rConstante; j++){
        nombresSolucion[j] = nuevoArreglo[j].nombre;
      }
      solucion solucionLocal;
      solucionLocal.nombres = nombresSolucion;
      solucionLocal.sumaPeso = sumaPesos;
      solucionLocal.sumaCalificacion = sumaCalificaciones;
      solucionLocal.cantidadNombres = rConstante;

      solucionesFinal = agregarSolucion(solucionesFinal, cantidadSoluciones, solucionLocal);
    }
    return;
  }
  /*LLamado recursivo
  */
  for(i = inicio; i <= largoArreglo-largo; i++){
    nuevoArreglo[rConstante-largo] = arreglo[i];
    combinaciones(arreglo, largo-1, i+1, nuevoArreglo, largoArreglo, rConstante, cantidadSoluciones,solucionesFinal,pesoMax);
  }
}


int main(int argc, char const *argv[]) {
  FILE *archivoEntrada;
  FILE *archivoSalida;
  int cantidadPersonas;
  int pesoMaximo;
  int i;
  int cantidadSoluciones = 0;
  int r;
  clock_t inicio, termino;
  inicio = clock();

  //Lectura archivo entrada
  archivoEntrada = fopen("entrada.in","r");
  fscanf(archivoEntrada,"%i %i",&cantidadPersonas,&pesoMaximo);

  //Arreglo de soluciones
  //solucion solucionesFinal[(int)pow(2,cantidadPersonas)-1];
  solucion * solucionesFinal = (solucion *)malloc(sizeof(solucion)*(int)pow(2,cantidadPersonas)-1);
  // Se define el arreglo de estructuras que se usa para resolver el problema
  postulante * casos = (postulante *)malloc(sizeof(postulante)*cantidadPersonas);

  // Se lee la data desde el archivo de entrada y asignamos a cada elemento de
  // la estructura
  for(i = 0; i<cantidadPersonas; i++){
    char * x = (char *)malloc(sizeof(char)*100);
    int peso, calificacion;
    fscanf(archivoEntrada, "%s", x);
    casos[i].nombre = x;
    fscanf(archivoEntrada, "%i", &peso);
    casos[i].peso = peso;
    fscanf(archivoEntrada, "%i", &calificacion);
    casos[i].calificacion = calificacion;
  }
  //Se crean las combinaciones posibles segun la cantidad de postulantes
  for(r=cantidadPersonas;r>=1;r--){
    postulante * casosNuevo = (postulante *)malloc(sizeof(postulante)*r);
    combinaciones(casos, r, 0, casosNuevo, cantidadPersonas, r, &cantidadSoluciones,solucionesFinal,pesoMaximo);
  }

  // Ordenamiento de la solucion final
  solucion * SolucionOrdenada = (solucion *)malloc(sizeof(solucion)*cantidadSoluciones);
  SolucionOrdenada = burbuja(solucionesFinal,cantidadSoluciones);

  //Generación archivo de salida
  archivoSalida = fopen("tripulacion.out","w");
  mostrarSoluciones(SolucionOrdenada, &cantidadSoluciones, archivoSalida);
  printf("Se ha creado archivo de salida tripulacion.out\n");
  fclose(archivoSalida);

  termino = clock();
  printf("Tiempo transcurrido: %f\n",(double)(termino - inicio)/ CLOCKS_PER_SEC);


  return 0;
}
