// assembler RE ver. 1.0   (translates users program to text with codes of commands)
// перед тем как что то делать помни что есть проблема с файлом меток из за которого прогу нужно запускать дважды
// метки не реализованы в суперкмд и не оформлен файл джампс

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <ctype.h>
#include <assert.h>

#define MAX_STR_LEN 100
#define WHERE printf ("File %s Line %d Function %s \n", __FILE__, __LINE__, __FUNCTION__);
#define MAX_POINTERS_NUM 20
#define TRASH 0b1111111111111111

#define DEF_CMD(name, num, code, is_arg) CMD_##name = num,

enum Enum_cmd
    {
    #include "supercmd.h"
    #include "jmps.h"
    CMD_DUMMY
    };
#undef DEF_CMD

/*typedef struct command
    {
    char cmd_name[MAX_STR_LEN];
    int cmd_num;
    }; */


int String_counter (FILE* file);
//void struct_cmd (FILE* file, command* cmd, int num_of_str);
void Word_read (FILE* file, char* s);
int my_strcmp(char* string1, char* string2);
void Translate (FILE* assemled, FILE* users_file, int string_count);
double Read_arg (FILE* file);
int is_rus_letter (char letter);
char my_tolower (char letter);
void Translate_with_metka(FILE* assembled, FILE* users_file, int string_count) ;
void Insert_metka(char* str, FILE* assembled, FILE* metki);

int main()
    {
        FILE* user_program = fopen ("program.txt", "r");   //users program that will be assemled

        FILE* prog_ass = fopen ("assembled.bin", "wb");

        printf("%d\n", String_counter(user_program));


        Translate (prog_ass, user_program, String_counter (user_program));

        rewind(prog_ass);
        printf("\n\nNOVII PROHOD - NOVAIA JIZNb \n\n");

        Translate_with_metka (prog_ass, user_program, String_counter (user_program));

       // double cmd[String_counter (commands)*2 - 2];

        //struct_cmd (commands, cmd, String_counter (commands) - 1);
        return 0;


    }



int String_counter (FILE* file)
    {
        char s[MAX_STR_LEN];
        int i = 0;

        while (fgets(s, 100, file) != NULL)
            {
                i++;
            }

        rewind (file);
        return i;
    }



/*void Struct_cmd (FILE* file, command* cmd, int num_of_str)


    {
        char s[MAX_STR_LEN];
        fgets(s, 100, file);
        for (int i = 0; i < num_of_str; i++)
            {
                Word_read (file, cmd[i].cmd_name);
                fscanf (file, "%d", &cmd[i].cmd_num);
            }
    }
 */


void Word_read (FILE* file, char* s)
    {
        char c = fgetc (file);

        int i=0;
        while ((c != ' ') && (c != '\n') && (c != EOF))
            {
                s[i] = c;

                c = fgetc (file);

                i++;
            }
        s[i] = '\0';
    }


/*void Translate (char* buf,double* cmd, FILE* file)




    {
        #define DEF_CMD (name, num, code)        \
            if (my_strcmp (buf, #name) == 0)      \         //MY_STRCMP FROM ONEGIN TO UNDERRATE UPPER LETTERS
            {                                      \
                cmd[0] = CMD_##name;                \
                if (!arg) fscanf (file, "%lg",c[1]);  \
            }
        else

        #include "supercmd.h"
        #undef DEF_CMD
    }
 */

void Translate (FILE* assembled, FILE* users_file, int string_count)    //MY_STRCMP FROM ONEGIN TO POFIG UPPER LETTERS.
    {
        //int metki[MAX_POINTERS_NUM];

        char commanda[MAX_STR_LEN];

        FILE* metki_file = fopen("metki_file.txt", "w");
        assert(metki_file);


        for (int i = 0; i < string_count;)
            {
            Word_read (users_file, commanda);

            #define DEF_CMD(name, num, code, is_arg)                  \
            if (my_strcmp (commanda, #name) == 0)                      \
                {                                                       \
                    int a = CMD_##name;                                  \
                    printf("Nomer cmd = %i \n", a);                       \
                    fwrite (&a, sizeof(int), 1, assembled);                \
                    if (is_arg == 0)                                        \
                        {                                                    \
                        double a = Read_arg (users_file);                     \
                        fwrite (&a, sizeof (double), 1, assembled);            \
                        }                                                       \
                }                                                                \
            else

            #include "supercmd.h"

            #undef DEF_CMD
    /*                                                            */

            #define DEF_CMD(name, num, code, is_arg)                  \
            if (my_strcmp (commanda, #name) == 0)                      \
                {                                                       \
                    int a = CMD_##name;                                  \
                    printf("Nomer cmd = %i \n", a);                       \
                    fwrite (&a, sizeof(int), 1, assembled);                \
                    if (is_arg == 0)                                        \
                        {                                                    \
                        char trash_str[MAX_STR_LEN];                          \
                        Word_read(users_file, trash_str);                      \
                        printf("poka ne smotrim %s\n", trash_str);              \
                                                                                 \
                        int a = TRASH ;                                           \
                        fwrite (&a, sizeof (int), 1, assembled);                   \
                        }                                                           \
                }                                                                    \
            else

            #include "jmps.h"

            #undef DEF_CMD

            if (commanda[0] == ':')
                    {
                        printf(" TUT METKA - %s \n" , commanda);
                        fprintf(metki_file, "%s %i\n", commanda, ftell(assembled));

                    }
            else



            {printf("ALO VSE O4EHb PLOXO: ALLERT COMMANDA NEOPOZNANA\n");}

            i++;

            }

        rewind(users_file);
        fclose(metki_file);

    }

double Read_arg (FILE* file)
    {
        char arg[MAX_STR_LEN] ;
        Word_read (file, arg);

        #define REGISTERSNUM(name, num)                      \
            if (strcmp(arg, #name) == 0) return (double)num ; \
            else

        #include "registersnames.h"

        #undef REGISTERSNUM

        return atof (arg);     //возвращает число дабл из строки арг (используется только если не подошло под названия регистров)
    }




int my_strcmp (char* string1, char* string2)
    {

    char* string_a =  string1;
    char* string_b =  string2;
    assert(string_a);

    while (1)
        {


        while (( !isalnum(*string_a) && (!is_rus_letter(*string_a)))  &&  (*string_a != '\0') )
            {
            string_a++;
            }

        while (( !isalnum(*string_b) && (!is_rus_letter(*string_b))) &&   (*string_b != '\0') )
            {

            string_b++;
            }

        if ( *string_a == '\0')
            {
            if (*string_b == '\0')
                {

                return 0;
                }
            else
                {

                 return -1;
                }
            }
        if ( *string_b == '\0')
            {

             return 1;
            }


        if (my_tolower(*string_a) - my_tolower(*string_b) != 0)
            {

            return (my_tolower(*string_a) - my_tolower(*string_b)) ;
            }
        string_a++;
        string_b++;
        }
    }


int is_rus_letter(char letter)
    {
    if ((letter >= 'А' && letter <= 'Я') || (letter >= 'а' && letter <= 'я') )
                {
                return 1;
                }
    else
        {
        return 0 ;
        }
    }

char my_tolower (char letter)
    {
    if (is_rus_letter(letter))
        {
         if (letter >= 'А' && letter <= 'Я')
            {
            return (letter + ('а' - 'А'));
            }
        }

    return tolower(letter);



    }




void Translate_with_metka(FILE* assembled, FILE* users_file, int string_count)    //MY_STRCMP FROM ONEGIN TO POFIG UPPER LETTERS.
    {
        //int metki[MAX_POINTERS_NUM];

        char commanda[MAX_STR_LEN];

        FILE* metki_file = fopen("metki_file.txt", "r");
        assert(metki_file);


        for (int i = 0; i < string_count;)
            {
            Word_read (users_file, commanda);

            #define DEF_CMD(name, num, code, is_arg)                  \
            if (my_strcmp (commanda, #name) == 0)                      \
                {                                                       \
                    int a = CMD_##name;                                  \
                    printf("Nomer cmd = %i \n", a);                       \
                    fwrite (&a, sizeof(int), 1, assembled);                \
                    if (is_arg == 0)                                        \
                        {                                                    \
                        double a = Read_arg (users_file);                     \
                        fwrite (&a, sizeof (double), 1, assembled);            \
                        }                                                       \
                }                                                                \
            else

            #include "supercmd.h"
            #undef DEF_CMD
    /*                                                            */

            #define DEF_CMD(name, num, code, is_arg)                  \
            if (my_strcmp (commanda, #name) == 0)                      \
                {                                                       \
                    int a = CMD_##name;                                  \
                    printf("Nomer cmd = %i \n", a);                       \
                    fwrite (&a, sizeof(int), 1, assembled);                \
                    if (is_arg == 0)                                        \
                        {                                                    \
                        char trash_str[MAX_STR_LEN];                          \
                        Word_read(users_file, trash_str);                      \
                        printf("Metka = %s \n" , trash_str);                    \
                        Insert_metka(trash_str, assembled, metki_file);          \
                        }                                                         \
                }                                                                  \
            else


            #include "jmps.h"

            #undef DEF_CMD



            if (commanda[0] == ':')
                    {
                     printf("Zdec pria4etcia metka kotoruu uje 4itali \n");
                    }
            else



            {printf("ALO VSE O4Ehb PLOXO: ALLERT COMMANDA NEOPOZNANA\n");}

            i++;

            }

        fclose(metki_file);

    }


void Insert_metka(char* str, FILE* assembled, FILE* metki)
    {
        rewind(metki);
        int num_metki;
        char metka[MAX_STR_LEN];

        while (1)
            {
                Word_read(metki, metka);

                if (my_strcmp(str, metka) == 0)
                    {

                        Word_read(metki, metka);
                        num_metki = atoi(metka);

                        printf("    num_metki = %i\n", num_metki);
                        fwrite(&num_metki, sizeof(int), 1, assembled);
                        break;
                    }

                else { Word_read(metki, metka);}
            }

    }
