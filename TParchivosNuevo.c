#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Plan
{
    char tipo;
    float montoPlan;
    int descuento;
} plan;

typedef struct Persona
{
    char nombre[10];
    char apellido[11];
    int edad;
    char sexo; // F M
    int mesesInscripto;
    plan plan;
    float montoAPagar;
} persona;

typedef struct nodoPlan
{
    plan miplan;
    struct nodoPlan *siguiente;
} nodoPlan;

typedef struct nodoPersona
{
    persona mipersona;
    struct nodoPersona *siguiente;
} nodoPersona;

int asignoDescuento(persona *mipersona);
float asignoMontoAPagar(persona *mipersona);
nodoPlan *creoNodoPlan(char miTipo, float miMonto, int miDescuento);
nodoPersona *creoNodoPersona(char minombre[10], char miapellido[11], int miedad, char misexo, int mimesesInscripto, char planTipo, nodoPlan **milistaPlanes);
int getdescuentoPorEdad(persona *mipersona);
int getdescuentoPorPlan(persona *mipersona);
void agregoPersona(nodoPersona *miNodoPersona, nodoPersona **listaPersonas);
void cargoPersonas(nodoPlan **listaPlanes, nodoPersona **listaPersonas);
void agregoPlan(nodoPlan *miNodoPlan, nodoPlan **listaPlanes);
void cargoPlanes(nodoPlan **listaPlanes);
void mostrarPlanes(nodoPlan **listaPlanes);         // Imprime por consola el plan
void escribirPlanes(nodoPlan **listaPlanes);        // Escribe en planesMarina.txt
void escribirPersonas(nodoPersona **listaPersonas); // Escribe en personasMarina.txt
void cobrarATodos(nodoPersona **lista);             // Asigna el montoAPagar a todas las personas
void imprimirListaCobro(nodoPersona **lista);       // Escribe en listaDeCobro.txt
// void leoPlanes(nodoPlan**listaPlanes);                                          //Lee de planesMarina.txt
void leoPersonas(nodoPersona **listaPersonas, nodoPlan **listaPlanes); // Lee de personasMarina.txt y agrega la persona del txt a la lista
plan getPlanByTipo(char planTipo, nodoPlan **listaPlanes);             // Recorre la lista de Planes y devuelve puntero al plan segun su Tipo
void limpioListaPersonas(nodoPersona **listaPersona);                  // Hace free de todos los nodos, de atras para adelante, deja solo el puntero doble

int main()
{
    nodoPersona *listaPersonas = NULL;
    nodoPlan *listaPlanes = NULL;

    cargoPlanes(&listaPlanes);
    mostrarPlanes(&listaPlanes); // Imprime en Terminal
    cargoPersonas(&listaPlanes, &listaPersonas);

    escribirPlanes(&listaPlanes);        // Escribe en planesMarina.txt
    cobrarATodos(&listaPersonas);        // Asigna Monto a Cobrar a todos los de la lista
    escribirPersonas(&listaPersonas);    // Escribe en personasMarina.txt
    limpioListaPersonas(&listaPersonas); // Elimina la lista pero queda el txt con las personas para despues leer
    printf("SE LIMPIO LA LISTA, PERO QUEDO EL TXT\n");

    leoPersonas(&listaPersonas, &listaPlanes); // Lee de personasMarina.txt y agrega la persona del txt a la lista
    cobrarATodos(&listaPersonas);              // Asigna Monto a Cobrar a todos los de la lista
    imprimirListaCobro(&listaPersonas);        // Escribe en listaDeCobro.txt
    return 0;
}

nodoPlan *creoNodoPlan(char miTipo, float miMonto, int miDescuento)
{
    nodoPlan *nuevoNodo = (nodoPlan *)malloc(sizeof(nodoPlan));
    nuevoNodo->miplan.tipo = miTipo;
    nuevoNodo->miplan.montoPlan = miMonto;
    nuevoNodo->miplan.descuento = miDescuento;
    nuevoNodo->siguiente = NULL;
    return nuevoNodo;
}

persona creoPersona(char minombre[10], char miapellido[11], int miedad, char misexo, int mimesesInscripto, char miplanTipo)
{
}
nodoPersona *creoNodoPersona(char minombre[10], char miapellido[11], int miedad, char misexo, int mimesesInscripto, char miplanTipo, nodoPlan **milistaPlanes)
{
    nodoPersona *nuevoNodoPersona = (nodoPersona *)malloc(sizeof(nodoPersona));
    strcpy(nuevoNodoPersona->mipersona.nombre, minombre);
    strcpy(nuevoNodoPersona->mipersona.apellido, miapellido);
    nuevoNodoPersona->mipersona.edad = miedad;
    nuevoNodoPersona->mipersona.sexo = misexo;
    nuevoNodoPersona->mipersona.mesesInscripto = mimesesInscripto;
    nuevoNodoPersona->mipersona.plan = getPlanByTipo(miplanTipo, milistaPlanes);
    nuevoNodoPersona->siguiente = NULL;
    return nuevoNodoPersona;
}

int asignoDescuento(persona *mipersona) // Me da el descuento total de la persona considerando edad, plan y antiguedad
{
    int descuentoAcumulado = 0;
    int descuentoPorEdad = getdescuentoPorEdad(mipersona);
    int descuentoPorPlan = getdescuentoPorPlan(mipersona);
    descuentoAcumulado = descuentoPorEdad + descuentoPorPlan;
    // printf("descuentoAcumulado %d \n", descuentoAcumulado);
    // printf("descuentoPorEdad %d \n", descuentoPorEdad);
    // printf("descuentoPorPlan %d \n", descuentoPorPlan);
    return descuentoAcumulado;
}
int getdescuentoPorEdad(persona *mipersona)
{
    int descuentoPorEdad = 0;
    if ((mipersona->edad < 12) || (65 < mipersona->edad))
    {
        descuentoPorEdad = 5;
    }
    else
    {
        descuentoPorEdad = 0;
    }
    return descuentoPorEdad;
}

int getdescuentoPorPlan(persona *mipersona)
{
    int descuentoPorPlan = 0;
    if (mipersona->mesesInscripto > 24)
    {
        descuentoPorPlan = mipersona->plan.descuento;
    }
    else
    {
        descuentoPorPlan = 0;
    }
    return descuentoPorPlan;
}

float asignoMontoAPagar(persona *mipersona) // Me da el valor de persona.montoAPagar considerando plan.monto y persona.descuento
{
    int descuento = asignoDescuento(mipersona);
    float mimontoAPagar = mipersona->plan.montoPlan - (mipersona->plan.montoPlan * descuento / 100);
    // printf(" El plan de %s sale  %.2f\n", mipersona->nombre, mipersona->plan.montoPlan);
    mipersona->montoAPagar = mimontoAPagar;
    // printf("%s tiene de que pagar %.2f\n", mipersona->nombre, mipersona->montoAPagar);
    return mimontoAPagar;
}

void agregoPersona(nodoPersona *miNodoPersona, nodoPersona **listaPersonas)
{
    // printf("el puntero de milista es %p\n", listaPersonas);
    if (*listaPersonas == NULL)
    {
        // printf("La lista estaba vacia\n");
        *listaPersonas = miNodoPersona;
        printf("Agregue a mi lista a %s\n", (*listaPersonas)->mipersona.nombre);
    }

    else if (*listaPersonas != NULL)
    {
        nodoPersona *puntero;
        puntero = *listaPersonas;

        while (puntero->siguiente)
        {
            puntero = puntero->siguiente;
            // printf("El puntero esta en el segundo que es %s\n", puntero->mipersona.nombre);
            // printf("Agregue a mi lista a %s\n", (*listaPersonas)->siguiente->mipersona.nombre);
        }
        puntero->siguiente = miNodoPersona;
        printf("Agregue a mi lista a %s\n", miNodoPersona->mipersona.nombre);

        // printf("188 Tiene un plan que sale  %.2f\n", miNodoPersona->mipersona.plan.montoPlan);
    }
}

void agregoPlan(nodoPlan *miNodoPlan, nodoPlan **listaPlanes)
{
    if (*listaPlanes == NULL)
    {
        *listaPlanes = miNodoPlan;
        // printf("Agregue el plan %c que sale %.2f\n", (*listaPlanes)->miplan.tipo, (*listaPlanes)->miplan.montoPlan);
    }
    else if (*listaPlanes != NULL)
    {
        nodoPlan *puntero;
        puntero = *listaPlanes;
        // printf("La lista no esta vacia\n");
        while (puntero->siguiente != NULL)
        {
            puntero = puntero->siguiente;
        }
        puntero->siguiente = miNodoPlan;
        // printf("Agregue el plan %c que sale %.2f\n", miNodoPlan->miplan.tipo, miNodoPlan->miplan.montoPlan);
    }
}

void mostrarPlanes(nodoPlan **listaPlanes)
{
    nodoPlan *puntero;
    puntero = *listaPlanes;
    if (puntero == NULL)
    {
        printf("Lista de planes vacia\n");
    }
    else if (puntero != NULL)
    {
        printf("Plan %c - Monto: %.2f - Descuento: %d porciento\n", puntero->miplan.tipo, puntero->miplan.montoPlan, puntero->miplan.descuento);
        while (puntero->siguiente)
        {
            puntero = puntero->siguiente;
            printf("Plan %c - Monto: %.2f - Descuento: %d porciento\n", puntero->miplan.tipo, puntero->miplan.montoPlan, puntero->miplan.descuento);
        }
    }
}

void escribirPlanes(nodoPlan **listaPlanes)
{

    FILE *archiPlan = fopen("planesMarina.txt", "w");
    if (archiPlan == NULL)
    {
        printf("Error al abrir el archivo planesMarina.txt\n");
    }
    else
    {
        if (listaPlanes != NULL)
        {
            nodoPlan *puntero;
            puntero = *listaPlanes;

            while (puntero->siguiente)
            {
                fprintf(archiPlan, "%c - %.2f - %d\n", puntero->miplan.tipo, puntero->miplan.montoPlan, puntero->miplan.descuento);
                puntero = puntero->siguiente;
            }
            fprintf(archiPlan, "%c - %.2f - %d\n", puntero->miplan.tipo, puntero->miplan.montoPlan, puntero->miplan.descuento);
            printf("Todos los planes fueron cargados en planesMarina.txt\n");
            fclose(archiPlan);
            printf("Se cerro el archivo planesMarina.txt\n");
        }
        else
        {
            printf("La lista esta vacia\n");
            fclose(archiPlan);
            printf("Se cerro el archivo planesMarina.txt\n");
        }
    }
}

void escribirPersonas(nodoPersona **listaPersonas)
{
    FILE *archi = fopen("personasMarina.txt", "w");
    if (archi != NULL)
    {
        nodoPersona *puntero;
        if (*listaPersonas == NULL)
        {
            printf("Lista vacia\n");
            fclose(archi);
            printf("Se cerro el archivo personasMarina.txt\n");
        }
        else
        {
            puntero = *listaPersonas;
            if (puntero->siguiente == NULL)
            {
                fprintf(archi, "%s - %s - %d - %c - %d - %c\n", puntero->mipersona.nombre, puntero->mipersona.apellido, puntero->mipersona.edad, puntero->mipersona.sexo, puntero->mipersona.mesesInscripto, puntero->mipersona.plan.tipo);
                // nombre " - " apellido " - " edad " - " sexo " - " mesesInscripto " - " plan
                fclose(archi);
                printf("Se cerro el archivo personasMarina.txt\n");
            }
            else
            {
                // printf("Primero es %s\n", puntero->mipersona.nombre);
                fprintf(archi, "%s - %s - %d - %c - %d - %c\n", puntero->mipersona.nombre, puntero->mipersona.apellido, puntero->mipersona.edad, puntero->mipersona.sexo, puntero->mipersona.mesesInscripto, puntero->mipersona.plan.tipo);

                // fprintf(archi, "%s", puntero->mipersona.nombre);
                while (puntero->siguiente)
                {
                    puntero = puntero->siguiente;
                    // fprintf(archi, "%s", puntero->mipersona.nombre);
                    fprintf(archi, "%s - %s - %d - %c - %d - %c\n", puntero->mipersona.nombre, puntero->mipersona.apellido, puntero->mipersona.edad, puntero->mipersona.sexo, puntero->mipersona.mesesInscripto, puntero->mipersona.plan.tipo);
                }
                fclose(archi);
                printf("Se cerro el archivo personasMarina.txt\n");
            }
        }
    }
    else
    {
        printf("No se pudo abrir el archivo personasMarina.txt\n");
    }
}

void cobrarATodos(nodoPersona **lista)
{
    nodoPersona *puntero;
    puntero = *lista;
    if (*lista == NULL)
    {
        printf("Lista vacia, no hay a quien cobrarle\n");
    }
    else
    {
        if (puntero->siguiente == NULL)
        {
            // printf("Estoy en el unico lugar\n");
            asignoMontoAPagar(&(puntero->mipersona));
        }
        else
        {
            asignoMontoAPagar(&(puntero->mipersona));
            while (puntero->siguiente)
            {
                puntero = puntero->siguiente;
                asignoMontoAPagar(&(puntero->mipersona));
            }
        }
    }
}

void imprimirListaCobro(nodoPersona **lista)
{
    FILE *archi = fopen("listaDeCobro.txt", "w");
    if (archi == NULL)
    {
        printf("No se pudo abrir el archivo listaDeCobro.txt\n");
    }
    else
    {
        nodoPersona *puntero;
        puntero = *lista;
        if (*lista == NULL)
        {
            printf("Lista vacia en listaDeCobro.txt\n");
            fclose(archi);
            printf("Se cerro el archivo listaDeCobro.txt\n");
        }
        else
        {
            puntero = *lista;
            fprintf(archi, "%s - %s - %d - %c - %d - %c - %.2f\n", puntero->mipersona.nombre, puntero->mipersona.apellido, puntero->mipersona.edad, puntero->mipersona.sexo, puntero->mipersona.mesesInscripto, puntero->mipersona.plan.tipo, puntero->mipersona.montoAPagar);
            while (puntero->siguiente)
            {
                puntero = puntero->siguiente;
                fprintf(archi, "%s - %s - %d - %c - %d - %c - %.2f\n", puntero->mipersona.nombre, puntero->mipersona.apellido, puntero->mipersona.edad, puntero->mipersona.sexo, puntero->mipersona.mesesInscripto, puntero->mipersona.plan.tipo, puntero->mipersona.montoAPagar);
            }
            fclose(archi);
            printf("Se cerro el archivo listaDeCobro.txt\n");
        }
    }
}

void leoPersonas(nodoPersona **listaPersonas, nodoPlan **listaPlanes) // Lee de personasMarina.txt y agrega la persona del txt a la lista
{
    printf("Leo de personas.txt\n");
    FILE *archi = fopen("personasMarina.txt", "r");
    if (archi == NULL)
    {
        printf("No se puede leer el archivo\n");
    }
    else
    {
        while (!feof(archi))
        {
            char minombre[10];
            char miapellido[10];
            int miedad;
            char misexo;
            int mimesesInscripto;
            char miplanTipo;
            fscanf(archi, "%s - %s - %d - %c - %d - %c\n", &minombre, &miapellido, &miedad, &misexo, &mimesesInscripto, &miplanTipo);

            plan miplan = getPlanByTipo(miplanTipo, listaPlanes);
            nodoPersona *nodo = creoNodoPersona(minombre, miapellido, miedad, misexo, mimesesInscripto, miplanTipo, listaPlanes);
            asignoMontoAPagar(&(nodo->mipersona));
            agregoPersona(nodo, listaPersonas);
        }
        printf("AGREGUE A TODOS DESDE EL TXT\n");
    }
}
plan getPlanByTipo(char planTipo, nodoPlan **listaPlanes)
{
    nodoPlan *puntero;
    puntero = *listaPlanes;
    if (puntero->miplan.tipo == planTipo)
    {
        // printf("El puntero esta en %c en getByTipo\n", puntero->miplan.tipo);
        return puntero->miplan;
    }
    else
    {
        while (puntero->siguiente)
        {
            puntero = puntero->siguiente;
            if (puntero->miplan.tipo == planTipo)
            {
                return (puntero->miplan);
            }
        }
    }
    printf("El plan %c no existe. No me rompas la base de datos!\n", planTipo);
}

void cargoPlanes(nodoPlan **listaPlanes)
{
    nodoPlan *a = creoNodoPlan('A', 10000, 25);
    nodoPlan *b = creoNodoPlan('B', 7500, 20);
    nodoPlan *c = creoNodoPlan('C', 5000, 15);
    nodoPlan *d = creoNodoPlan('D', 2500, 10);
    agregoPlan(a, listaPlanes);
    agregoPlan(b, listaPlanes);
    agregoPlan(c, listaPlanes);
    agregoPlan(d, listaPlanes);
}
void cargoPersonas(nodoPlan **listaPlanes, nodoPersona **listaPersonas)
{
    agregoPersona((creoNodoPersona("Marina", "Carracedo", 20, 'F', 2, 'A', listaPlanes)), listaPersonas); // Edad sin descuento Antiguedad sin descuento
    agregoPersona((creoNodoPersona("Pablo", "Ramis", 45, 'M', 26, 'A', listaPlanes)), listaPersonas);     // Edad sin descuento Antiguedad con descuento
    agregoPersona((creoNodoPersona("Luciano", "Diamand", 5, 'M', 2, 'A', listaPlanes)), listaPersonas);   // Edad con descuento Antiguedad sin descuento
    // agregoPersona((creoNodoPersona("Josefa", "Panettieri", 95, 'F', 26, 'A', listaPlanes)), listaPersonas); // Edad con descuento Antiguedad con descuento
    agregoPersona((creoNodoPersona("Gabriela", "Perez", 20, 'F', 2, 'C', listaPlanes)), listaPersonas);    // Edad sin descuento Antiguedad sin descuento
    agregoPersona((creoNodoPersona("Gonzalo", "Pinat", 20, 'M', 26, 'C', listaPlanes)), listaPersonas);    // Edad sin descuento Antiguedad con descuento
    agregoPersona((creoNodoPersona("Federico", "Aroca", 5, 'M', 2, 'C', listaPlanes)), listaPersonas);     // Edad con descuento Antiguedad sin descuento
    agregoPersona((creoNodoPersona("Elisabeth", "Almada", 95, 'F', 26, 'C', listaPlanes)), listaPersonas); // Edad con descuento Antiguedad con descuento
}

void limpioListaPersonas(nodoPersona **listaPersona)
{
    nodoPersona *puntero;
    puntero = *listaPersona;
    if (puntero == NULL)
    {
        printf("No hay nadie para Liberar\n");
    }
    else if (puntero != NULL)
    {
        if (puntero->siguiente != NULL)
        {
            while (puntero->siguiente->siguiente)
            {
                // printf("Hay alguien mas...\n");
                puntero->siguiente = puntero->siguiente->siguiente;
            }
            // printf(" free\n");
            free(puntero->siguiente);
            puntero->siguiente = NULL;
            limpioListaPersonas(listaPersona);
        }
        else if (puntero->siguiente == NULL)
        {
            if (puntero->siguiente)
            {
                // printf("Puntero 455 %s\n", puntero->mipersona.nombre);
                free(puntero->siguiente);
                // printf("Se liberando el ultimo...\n");
                puntero->siguiente = NULL;
                // printf("461ultimo en NULL\n");
                free(puntero);
                // printf("463Se liberando el ultimo...\n");
                puntero = NULL;
                // printf("ultimo en NULL\n");
                limpioListaPersonas(listaPersona);
            }
            else
            {
                free(puntero);
                // printf("463Se liberando el ultimo...\n");
                puntero = NULL;
                // printf("ultimo en NULL\n");
            
                *listaPersona = NULL;

            }
        }
    }
}