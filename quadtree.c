#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

typedef struct QuadtreeNode // element al vectorului
{
    unsigned char blue, green, red;
    uint32_t area;
    int32_t top_left, top_right;
    int32_t bottom_left, bottom_right;
} __attribute__((packed)) QuadtreeNode;

typedef struct pixel // pixel
{
    unsigned char red, green, blue;
} pixel;

typedef struct Node // nod al arborelui
{
    unsigned char red, green, blue;
    int size;
    struct Node *top_left;
    struct Node *top_right;
    struct Node *bottom_left;
    struct Node *bottom_right;
} Node;

// functie pentru medie
void average(pixel **p, unsigned int size, int x, int y, int64_t *red, int64_t *green, int64_t *blue)
{
    int i, j;
    *red = 0;
    *green = 0;
    *blue = 0;

    for (i = x; i < x + size; i++)
        for (j = y; j < y + size; j++)
        {
            *red += (int64_t)p[i][j].red;
            *green += (int64_t)p[i][j].green;
            *blue += (int64_t)p[i][j].blue;
        }

    (*red) /= (size * size);
    (*green) /= (size * size);
    (*blue) /= (size * size);
}

void init(Node *root) // functie pentru alocare de memorie pentru fii
{
    root->top_right = (Node *)malloc(sizeof(Node));
    root->top_left = (Node *)malloc(sizeof(Node));
    root->bottom_left = (Node *)malloc(sizeof(Node));
    root->bottom_right = (Node *)malloc(sizeof(Node));
}

// functie pentru trecerea datelor din matrice in arbore
void div_p(pixel **p, unsigned int size, int x, int y, int prag, Node *root, int *num_nodes, int *num_leafs)
{
    int64_t i, j, scor = 0;
    int64_t red, green, blue;

    if (size)
    {
        average(p, size, x, y, &red, &green, &blue);

        // se calculeaza scorul dupa formula
        for (i = x; i < x + size; i++)
            for (j = y; j < y + size; j++)
                scor += (red - p[i][j].red) * (red - p[i][j].red) + (green - p[i][j].green) * (green - p[i][j].green) + (blue - p[i][j].blue) * (blue - p[i][j].blue);
        scor = scor / (3 * size * size);
    }

    if (scor > prag) // se continua impartirea
    {
        init(root);
        div_p(p, size / 2, x, y, prag, root->top_left, num_nodes, num_leafs);
        div_p(p, size / 2, x, y + size / 2, prag, root->top_right, num_nodes, num_leafs);
        div_p(p, size / 2, x + size / 2, y + size / 2, prag, root->bottom_right, num_nodes, num_leafs);
        div_p(p, size / 2, x + size / 2, y, prag, root->bottom_left, num_nodes, num_leafs);
    }
    else // s-a terminat impartirea matricei si s-a ajuns la frunze
    {
        root->top_left = NULL;
        root->top_right = NULL;
        root->bottom_right = NULL;
        root->bottom_left = NULL;
        (*num_leafs)++;
    }

    // se completeaza datele pentru nodul curent
    root->red = (unsigned char)red;
    root->green = (unsigned char)green;
    root->blue = (unsigned char)blue;
    root->size = size * size;
    (*num_nodes)++;

    return;
}

// functie pentru completarea datelor din arbore in vector
void tree_to_vector(Node *root, QuadtreeNode *vector, int *num_elem, int32_t *x, int indice, int *indice_prev)
{
    if (root == NULL) // nodul anterior este frunza
        *x = -1;
    else
    {
        vector[*num_elem].red = root->red;
        vector[*num_elem].green = root->green;
        vector[*num_elem].blue = root->blue;
        vector[*num_elem].area = root->size;

        (*num_elem)++;

        tree_to_vector(root->top_left, vector, num_elem, x, *num_elem, indice_prev);
        if (*x == -1) // nodul este frunza => se completeaza indicii cu -1
        {
            vector[*num_elem - 1].top_right = -1;
            vector[*num_elem - 1].bottom_right = -1;
            vector[*num_elem - 1].bottom_left = -1;
            vector[*num_elem - 1].top_left = -1;
            *x = 1;
        }
        else
            vector[indice].top_left = *indice_prev; // nodul nu este frunza => se completeaza indicele cu indicele nodului vizitat anterior

        tree_to_vector(root->top_right, vector, num_elem, x, *num_elem, indice_prev);
        if (*x == -1) // nodul este frunza => se completeaza indicii cu -1
        {
            vector[*num_elem - 1].top_left = -1;
            vector[*num_elem - 1].bottom_right = -1;
            vector[*num_elem - 1].bottom_left = -1;
            vector[*num_elem - 1].top_right = -1;
            *x = 1;
        }
        else
            vector[indice].top_right = *indice_prev; // nodul nu este frunza => se completeaza indicele cu indicele nodului vizitat anterior

        tree_to_vector(root->bottom_right, vector, num_elem, x, *num_elem, indice_prev);
        if (*x == -1) // nodul este frunza => se completeaza indicii cu -1
        {
            vector[*num_elem - 1].top_right = -1;
            vector[*num_elem - 1].bottom_left = -1;
            vector[*num_elem - 1].bottom_left = -1;
            vector[*num_elem - 1].bottom_right = -1;
            *x = 1;
        }
        else
            vector[indice].bottom_right = *indice_prev; // nodul nu este frunza => se completeaza indicele cu indicele nodului vizitat anterior

        tree_to_vector(root->bottom_left, vector, num_elem, x, *num_elem, indice_prev);
        if (*x == -1) // nodul este frunza => se completeaza indicii cu -1
        {
            vector[*num_elem - 1].top_right = -1;
            vector[*num_elem - 1].bottom_right = -1;
            vector[*num_elem - 1].top_left = -1;
            vector[*num_elem - 1].bottom_left = -1;
            *x = 1;
        }
        else
            vector[indice].bottom_left = *indice_prev; // nodul nu este frunza => se completeaza indicele cu indicele nodului vizitat anterior

        *indice_prev = indice; // se retine indicele pentru a completa datele nodului anterior
    }
}

// functie pentru completarea datellor din vector in arbore
void vector_to_tree(QuadtreeNode *vector, Node *root, int poz)
{
    // se completeaza datele
    root->red = vector[poz].red;
    root->green = vector[poz].green;
    root->blue = vector[poz].blue;
    root->size = sqrt(vector[poz].area);

    if (vector[poz].top_left != -1) // nodul nu este frunza
    {
        init(root);

        // se continua parcurgerea
        vector_to_tree(vector, root->top_left, vector[poz].top_left);
        vector_to_tree(vector, root->top_right, vector[poz].top_right);
        vector_to_tree(vector, root->bottom_left, vector[poz].bottom_left);
        vector_to_tree(vector, root->bottom_right, vector[poz].bottom_right);
    }
    else // nodul este frunza
    {
        root->top_left = NULL;
        root->top_right = NULL;
        root->bottom_left = NULL;
        root->bottom_right = NULL;
    }
}

// functie pentru completarea datelor din arbore in matrice
void tree_to_matrix(Node *root, pixel **p, int size, int x, int y)
{
    if (root->top_left != NULL) // nodul nu este frunza => se continua parcurgerea
    {
        tree_to_matrix(root->top_left, p, size / 2, x, y);
        tree_to_matrix(root->top_right, p, size / 2, x, y + size / 2);
        tree_to_matrix(root->bottom_right, p, size / 2, x + size / 2, y + size / 2);
        tree_to_matrix(root->bottom_left, p, size / 2, x + size / 2, y);
    }
    else // nodul este frunza => se completeaza matricea
    {
        int i, j;

        for (i = x; i < x + size; i++)
            for (j = y; j < y + size; j++)
            {
                p[i][j].red = root->red;
                p[i][j].green = root->green;
                p[i][j].blue = root->blue;
            }
    }
}

// functie pentru oglindirea pe orizontala
void mirror_h(Node *root)
{
    if (root != NULL)
    {
        Node *aux;

        // se inverseaza primele doua noduri de pe nivel
        aux = root->top_left;
        root->top_left = root->top_right;
        root->top_right = aux;

        // se inverseaza ultimele doua noduri de pe nivel
        aux = root->bottom_right;
        root->bottom_right = root->bottom_left;
        root->bottom_left = aux;

        // se continua parcurgerea
        mirror_h(root->top_left);
        mirror_h(root->top_right);
        mirror_h(root->bottom_right);
        mirror_h(root->bottom_left);
    }
    else
        return;
}

// functie pentru oglindirea pe orizontala
void mirror_v(Node *root)
{
    if (root != NULL)
    {
        Node *aux;

        // se inverseaza primul si ultimul nod de pe nivel
        aux = root->top_left;
        root->top_left = root->bottom_left;
        root->bottom_left = aux;

        // se inverseaza nodurile mijlocii de pe nivel
        aux = root->top_right;
        root->top_right = root->bottom_right;
        root->bottom_right = aux;

        // se continua parcurgerea
        mirror_v(root->top_left);
        mirror_v(root->top_right);
        mirror_v(root->bottom_right);
        mirror_v(root->bottom_left);
    }
    else
        return;
}

// functie pentru eliberarea memoriei pentru arbore
void free_tree(Node *root)
{
    if (root != NULL)
    {
        free_tree(root->top_left);
        free_tree(root->top_right);
        free_tree(root->bottom_right);
        free_tree(root->bottom_left);
        free(root);
    }
    else
        return;
}

// functie pentru compresie
void task1(FILE *input, FILE *output, int prag)
{
    char file_type[4], new_line;
    unsigned int i, j, num_max;
    int num_elem = 0, indice_prev = 0, num_nodes = 0, num_leafs = 0, x = 0;
    unsigned int width, height;

    fscanf(input, "%s%u%u%u", file_type, &width, &height, &num_max); // se citesc datele
    fread(&new_line, sizeof(char), 1, input);                        // se citeste ultimul enter

    Node *root = (Node *)malloc(sizeof(Node));

    pixel **p = (pixel **)malloc(height * sizeof(pixel *));
    for (i = 0; i < height; i++)
        p[i] = (pixel *)malloc(width * sizeof(pixel));

    // se citeste matricea de pixeli
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            fread(&p[i][j].red, sizeof(char), 1, input);
            fread(&p[i][j].green, sizeof(char), 1, input);
            fread(&p[i][j].blue, sizeof(char), 1, input);
        }
    }

    div_p(p, width, 0, 0, prag, root, &num_nodes, &num_leafs); // se formeaza arborele

    QuadtreeNode *vector = (QuadtreeNode *)malloc(num_nodes * sizeof(QuadtreeNode));
    tree_to_vector(root, vector, &num_elem, &x, 0, &indice_prev); // se formeaza vectorul

    fwrite(&num_leafs, sizeof(int), 1, output);              // se scrie nr de frunze
    fwrite(&num_nodes, sizeof(int), 1, output);              // se scrie nr de noduri
    fwrite(vector, sizeof(QuadtreeNode), num_nodes, output); // se scrie vectorul

    // se elibereaza memoria
    for (i = 0; i < height; i++)
        free(p[i]);
    free(p);
    free(vector);
    free_tree(root);
}

// functie pentru decompresie
void task2(FILE *input, FILE *output)
{
    int num_nodes, num_leafs, i;

    fread(&num_leafs, sizeof(int), 1, input); // se citeste nr de frunze
    fread(&num_nodes, sizeof(int), 1, input); // se citeste nr de noduri

    Node *root = (Node *)malloc(sizeof(Node));
    QuadtreeNode *vector = (QuadtreeNode *)malloc(num_nodes * sizeof(QuadtreeNode));

    for (i = 0; i < num_nodes; i++) // se citeste vectorul
    {
        fread(&(vector[i].blue), sizeof(char), 1, input);
        fread(&(vector[i].green), sizeof(char), 1, input);
        fread(&(vector[i].red), sizeof(char), 1, input);
        fread(&(vector[i].area), sizeof(uint32_t), 1, input);
        fread(&(vector[i].top_left), sizeof(int32_t), 1, input);
        fread(&(vector[i].top_right), sizeof(int32_t), 1, input);
        fread(&(vector[i].bottom_left), sizeof(int32_t), 1, input);
        fread(&(vector[i].bottom_right), sizeof(int32_t), 1, input);
    }

    unsigned int width = sqrt(vector[0].area), height = sqrt(vector[0].area);

    vector_to_tree(vector, root, 0); // se construieste arborele

    pixel **p = (pixel **)malloc(height * sizeof(pixel *));
    for (i = 0; i < height; i++)
        p[i] = (pixel *)malloc(width * sizeof(pixel));

    tree_to_matrix(root, p, width, 0, 0); // se construieste matricea

    char file_type[4] = "P6\n", num_max[5] = "255\n";

    fwrite(file_type, sizeof(char), 3, output); // se scrie tipul fisierului
    fprintf(output, "%d %d\n", width, height);  // se scrie dimensiunea
    fwrite(num_max, sizeof(char), 4, output);   // se scrie dimensiunea maxima a unei culori din pixel

    for (i = 0; i < height; i++)
        fwrite(p[i], sizeof(pixel), width, output); // se scrie matricea de pixeli

    // se elibereaza memoria
    for (i = 0; i < height; i++)
        free(p[i]);
    free(p);
    free(vector);
    free_tree(root);
}

// functie pentru oglindire
void task3(FILE *input, FILE *output, char type, int prag)
{
    char file_type[11], new_line;
    unsigned int width, height, i, j, num_max;
    int num_nodes = 0, num_leafs = 0;

    fscanf(input, "%s%u%u%u", file_type, &width, &height, &num_max); // se citesc datele
    fread(&new_line, sizeof(char), 1, input);                        // se citeste ultimul enter

    Node *root = (Node *)malloc(sizeof(Node));

    pixel **p = (pixel **)malloc(height * sizeof(pixel *));
    for (i = 0; i < height; i++)
        p[i] = (pixel *)malloc(width * sizeof(pixel));

    // se citeste matricea de pixeli
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            fread(&p[i][j].red, sizeof(char), 1, input);
            fread(&p[i][j].green, sizeof(char), 1, input);
            fread(&p[i][j].blue, sizeof(char), 1, input);
        }
    }

    div_p(p, width, 0, 0, prag, root, &num_nodes, &num_leafs); // se formeaza arborele

    if (type == 'v')
        mirror_v(root); // arborele se oglindeste pe verticala
    if (type == 'h')
        mirror_h(root); // arborele se oglindeste pe orizontala

    tree_to_matrix(root, p, width, 0, 0); // se formeaza noua matrice

    char num_max_c[5] = "255\n", file_type2[4] = "P6\n";

    fwrite(file_type2, sizeof(char), 3, output); // se scrie tipul fisierului
    fprintf(output, "%d %d\n", width, height);   // se scrie dimensiunea
    fwrite(num_max_c, sizeof(char), 4, output);  // se scrie dimensiunea maxima a unei culori din pixel

    for (i = 0; i < height; i++)
        fwrite(p[i], sizeof(pixel), width, output); // se scrie matricea de pixeli

    // se elibereaza memoria
    for (i = 0; i < height; i++)
        free(p[i]);
    free(p);
    free_tree(root);
}

int main(int argc, char *argv[])
{
    char type, infile_name[30], outfile_name[30];
    int i, prag;

    if (strcmp(argv[1], "-c") == 0)
    {
        prag = 0;
        for (i = 0; i < strlen(argv[2]); i++)
            prag = prag * 10 + (argv[2][i] - '0'); // se afla pragul

        strcpy(infile_name, argv[3]);
        strcpy(outfile_name, argv[4]);

        FILE *input = fopen(infile_name, "rb");   // se deschide fisierul de intrare
        FILE *output = fopen(outfile_name, "wb"); // se deschide fisierul de iesire

        task1(input, output, prag);

        fclose(input);  // se inchide fisierul de intrare
        fclose(output); // se inchide fisierul de iesire
    }

    if (strcmp(argv[1], "-d") == 0)
    {
        strcpy(infile_name, argv[2]);
        strcpy(outfile_name, argv[3]);

        FILE *input = fopen(infile_name, "rb");   // se deschide fisierul de intrare
        FILE *output = fopen(outfile_name, "wb"); // se deschide fisierul de iesire

        task2(input, output);

        fclose(input);  // se inchide fisierul de intrare
        fclose(output); // se inchide fisierul de iesire
    }

    if (strcmp(argv[1], "-m") == 0)
    {
        strcpy(infile_name, argv[4]);
        strcpy(outfile_name, argv[5]);

        type = argv[2][0];

        prag = 0;
        for (i = 0; i < strlen(argv[3]); i++)
            prag = prag * 10 + (argv[3][i] - '0'); // se afla pragul

        FILE *input = fopen(infile_name, "rb");   // se deschide fisierul de intrare
        FILE *output = fopen(outfile_name, "wb"); // se deschide fisierul de iesire

        task3(input, output, type, prag);

        fclose(input);  // se inchide fisierul de intrare
        fclose(output); // se inchide fisierul de iesire
    }

    return 0;
}