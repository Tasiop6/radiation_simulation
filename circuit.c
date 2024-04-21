/*****************************************************/
/*                                                   */
/* Created By George Papageorgiou                    */
/* AEM: 2600                                         */
/* Sxediasmos kiklwmatwn gia iontizonta perivalonta  */
/*                                                   */
/*****************************************************/ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 512
#define MAX_FANOUT 50

struct temp_gate{
    char temp_gate_name[50];
    char temp_gate_inputs[200];
    char temp_gate_output[100];
    char temp_gate_type[50];
    int input, output;
    struct temp_gate *next;
};

struct gate{
    char gate_name[50];
    char gate_inputs[200];
    char gate_output[100];
    char gate_type[50];
    int input, output;
    //struct temp_gate *next;
    //struct wire *next[10] = NULL;
    //I will point to node n
    struct wire *inputs[50]; //Remember to Initialize to NULL
    struct wire *outputs[2];
};

//For wires
struct wire{
    char node_name[50];
    int fanout_count;
    struct gate *comingfrom;
    struct gate *goingto[50];
};

//Funtrions
//Function to set \0 string
void SetZero(char str[], int size);
void BasicDataSpliter(struct temp_gate *ptr, char tosplit[]);
void InputOutputSpliter(struct temp_gate *ptr, char tosplit[]);
char * StringSplit(char tosplit[]);
int FindInputOutputs(char stringforsearch[]);
struct temp_gate *CreateInitialList(char *input, char *output, char *wires);
struct gate *CreateFinalList(struct temp_gate *firstptr);
struct temp_gate *Find(struct temp_gate *ptr, char stringforsearch[], struct temp_gate *value[]);

//Free pointers
void FreeMem(struct temp_gate **ptr);

int main(int argc, char *argv[]){
    char input[500];
    char output[500];
    char wires[500];
    
    struct temp_gate *templisthead;
    //struct gate *head;
    
    SetZero(input, 500);
    SetZero(output, 500);
    SetZero(wires, 500);
    
    templisthead = CreateInitialList(input, output, wires);
    if(templisthead == NULL){
        printf("Oups, spaceship is launching!\n");
        return -1;
    }
    
    //Creting the correct list in here
//     char search[50];
//     SetZero(search, 50);
//     int pos=0, i;
//     
//     while(output[pos] != 'p'){
//         pos++;
//     }
//     while(output[pos+4] != ';'){
//         search[i] = output[pos+4];
//         pos++;
//         i++;
//     }
    
    //Create new node for the final list with wires
    //head = CreateFinalList(templisthead);
    //if(head == NULL){
    //    printf("Oups, spaceship is launching!\n");
    //    return -1;
    //}
    
    //Create new wires and link them together as the list comes
    
    //Create them
    
    //Delete prev gate from temp list
    
    
    //print list here
    struct temp_gate *curr = templisthead;
    
    while(curr!=NULL){
       printf("%s\n%s\n%s\n%s\n", curr->temp_gate_name, curr->temp_gate_inputs, curr->temp_gate_output, curr->temp_gate_type);
       printf("\n"); 
       curr = curr->next;
    }
    printf("%s\n%s\n%s\n", input, output, wires); 
    
    struct temp_gate *value[40];
    
    curr = Find(templisthead, "n82", value);
    if(curr != NULL){
        printf("Fuck\n");
    }
    
    //Free the list
    FreeMem(&templisthead);
    
    return 0;
}
/************************************************ DO NOT TOUCH WORKING PERFECTLY *********************************************************************/
//Set string to \0
void SetZero(char str[], int size){
    int counter;
    
    for(counter=0; counter<size; counter++){
        str[counter] = '\0';
    }
    
}

void BasicDataSpliter(struct temp_gate *ptr, char tosplit[]){
    char temp[100];
    int counter=0, counter1=0;
    
    SetZero(temp,100);
    
    //First get the name
    while(tosplit[counter] != '('){
        temp[counter] = tosplit[counter];
        counter++;
    }
    //Delete last space from temp_gate name
    temp[counter] = '\0';
    while(counter1 < counter){
        temp[counter1] = temp[counter1+2];
        counter1++;
    }
    
    //Set it to the temp_gate_name
    strcpy(ptr->temp_gate_name, temp);
    
    //Clear temp string
    SetZero(temp, 100);
    
    //Got the Substring
    if(tosplit[counter] == ' '){
        counter++;
    }
    
    counter1 = 0;
    while(tosplit[counter] != ';'){
        temp[counter1] = tosplit[counter];
        counter++;
        counter1++;
    }
    //Drop space);
    temp[counter1+2] = ';';
    
    //Split input-output and type
    InputOutputSpliter(ptr, temp);
}

void InputOutputSpliter(struct temp_gate *ptr, char tosplit[]){
    int counter1=0;
    int counter=0, inp;
    char Q = 'Q';
    char Z = 'Z';
    char temp[500];
    
    SetZero(temp,500);
    
    while(1){
        if(tosplit[counter] == Q){
            break;
        }else if(tosplit[counter] == Z){
            break;
        }
        counter++;
    }
    
    inp=counter;
    counter1 = 0;
    
    while(tosplit[counter-1] != ';'){
        temp[counter1] = tosplit[counter-1];
        counter++;
        counter1++;
    }
    
    //Drop space);
    temp[counter1-3] = '\0';

    for(counter=0; counter<counter1; counter++){
        temp[counter] = temp[counter+1];
    }
    char output[200];
    SetZero(output, 200);
    strcpy(output, temp);
    strcat(output, ";");
    strcpy(ptr->temp_gate_output, StringSplit(output));
    
    SetZero(temp, 100);
    for(counter=0; counter<inp; counter++){
        temp[counter] = tosplit[counter];
    }
    //Drop space
    for(counter=0; counter<inp; counter++){
        temp[counter] = temp[counter+1];
    }
    temp[counter] = '\0';
    strcat(temp,";");
    
    strcpy(ptr->temp_gate_inputs, StringSplit(temp));
    
    //Setting type
    char type[30];
    strcpy(type, ptr->temp_gate_name);
    //You have 2 spaces before a letter
    if(type[0] == 'D'){
        strcpy(ptr->temp_gate_type ,"D_Flif_Flop");
    }
    else if(type[0] == 'O'){
        strcpy(ptr->temp_gate_type, "OR");
    }
    else if(type[0] == 'I'){
        strcpy(ptr->temp_gate_type, "Inverter");
    }
    else if(type[0] == 'A'){
        strcpy(ptr->temp_gate_type, "AND");
    }
    else if(type[0] == 'N'){
        if(type[1] == 'A'){
            strcpy(ptr->temp_gate_type, "NAND");
        }
        else if(type[1] == 'O'){
            strcpy(ptr->temp_gate_type, "NOR");
        }
    }
}

char * StringSplit(char tosplit[]){
    char temp[500];
    char splited[500];
    int counter = 0;
    int counter1 = 0;
    
    SetZero(temp, 500);
    SetZero(splited, 500);
    
    while(tosplit[counter] != ';'){
        if(tosplit[counter] == '('){
            counter++;
            counter1=0;
            while(tosplit[counter] != ')'){
                temp[counter1] = tosplit[counter];
                counter++;
                counter1++;
            }
            strcat(splited, temp);
            strcat(splited, " ");
            SetZero(temp, 500);
        }
        counter++;
    }
    
    SetZero(tosplit, 500);
    strcpy(tosplit, splited);
    
    return tosplit;
}

//Clear memory droping the real head
void FreeMem(struct temp_gate **ptr){
    struct temp_gate *curr;
    struct temp_gate *next;
    
    curr = *ptr;
    
    while(curr != NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }
    
    *ptr = NULL;
    
    return;
}
/******************************************************************************************************************************************************/

int FindInputOutputs(char stringforsearch[]){
    int counter=0, space=0;
    
    while(counter<stringforsearch[counter]){
        if(stringforsearch[counter] == ' '){
            space++;
        }
        counter++;
    }
    
    return space;
}

struct temp_gate *CreateInitialList(char *input, char *output, char *wires){
    //File pointer 
    FILE *fp;
    char str[SIZE]; //String to read from file
    
    //String for the circuit input - output - wire
    char inputs[200];
    char outputs[50];
    char wire[200];
    char temp[50];
    char readtemp[600];
    char read[600];
    
    //Which line from file i read starting from input(0)
    int counter, lineread = 0; 
    int flag=0;
    
    //Head of temp linked list
    struct temp_gate *head = NULL;
    struct temp_gate *tnode;
    struct temp_gate *curr;
    
    //Initialize everything to \0
    SetZero(str, SIZE);
    SetZero(inputs, 200);
    SetZero(outputs, 50);
    SetZero(wire, 200);
    SetZero(temp,50);
    SetZero(readtemp, 600);
    SetZero(read, 600);
    
    // opening file for reading
    fp = fopen("s298_mapped.v" , "r");
    if(fp == NULL) {
       perror("Error opening file");
      
      return NULL;
    }
   
   while(fgets(str, SIZE, fp)!= NULL){
       
       strcpy(readtemp,str);
       
       if(readtemp[0] == '/'){
            SetZero(str, SIZE);
            SetZero(readtemp, 600);
            continue;
        }
        //Start code here for structure!!!!!
        
        //Throw first line saying module
        //Check first only for m only for speed
        if(readtemp[0] == 'm'){
            SetZero(temp,10);
            for(counter=0; counter<6; counter++){
                temp[counter] = readtemp[counter];
            }
            if(!strcmp(temp,"module")){
                lineread = 0;
                continue;
            }
        }
        
        //Throw last line saying endmodule
        if(readtemp[0] == 'e'){
            SetZero(readtemp,10);
            for(counter=0; counter<9; counter++){
                temp[counter] = readtemp[counter];
            }
            if(!strcmp(temp,"endmodule")){
                break;
            }
        }
        
        //do it until \n so we can track ,
        counter=0;
        while(readtemp[counter] != '\n'){
            if(readtemp[counter] == ';'){break;}
            counter++;
        }
        //SetZero(readtemp, 300);
        if(readtemp[counter-1] == ','){
            strcat(read, readtemp);
            flag = 1;
            continue;
        }
        if(flag == 1){
            strcat(read, readtemp);
        }
        
        if(flag == 0){
            strcpy(read, readtemp);
        }
        
        
        //Read inputs
        if(lineread == 0){
            strcpy(inputs, read);
            SetZero(str, SIZE);
            SetZero(readtemp, 600);
            SetZero(read, 600);
            lineread++;
            continue;
        }
        //Read output
        if(lineread == 1){
            strcpy(outputs, read);
            SetZero(str, SIZE);
            SetZero(readtemp, 600);
            SetZero(read, 600);
            lineread++;
            continue;
        }
        //Read wire
        if(lineread == 2){
            strcpy(wire, read);
            SetZero(str, SIZE);
            SetZero(readtemp, 600);
            SetZero(read, 600);
            lineread++;
            continue;
        }
        //I have saved input,output and wires so
        //Create temp_gate Structs frome here
        //Just building the list as given from file
        //I will rebuild it later
        
        //Drop empty lines no line will have length less than 2
        if(strlen(read) < 2){
            lineread++;
            continue;
        }
        
        //lineread is 4 means i just started reading the temp_gates
        if(lineread == 4){
            head = (struct temp_gate *)malloc(sizeof(struct temp_gate));
            if(head == NULL){
                printf("Error alocating memmory for the temp_gate!!!\nExiting...\n");
                return NULL;
            }
            BasicDataSpliter(head, read);
            head->next = NULL;
            lineread++;
            SetZero(str, SIZE);
            SetZero(readtemp, 600);
            SetZero(read, 600);
            tnode = head; 
            continue;
        }
        curr = (struct temp_gate *)malloc(sizeof(struct temp_gate));
        if(curr == NULL){
            printf("Error alocating memmory for the temp_gate!!!\nExiting...\n");
            return NULL;
        }
        
        //Fix first list
        tnode->next = curr;
        curr->next = NULL;
        BasicDataSpliter(curr, read);
        lineread++;
        tnode = curr;
        
        flag = 0;
        
        SetZero(str, SIZE);
        SetZero(readtemp, 600);
        SetZero(read, 600);
        
    }
    strcpy(input,inputs);
    strcpy(output, outputs);
    strcpy(wires, wire);

    //Close pointer to file
    fclose(fp);
    
    return head;
}

/*struct gate *CreateFinalList(struct temp_gate *firstptr){
    int counter,i=0, conterpos;
    
    struct temp_gate *ptr, *curr;
    struct gate *head, *pos;
    struct wire *create, *wirehead;
    
    curr = firstptr;
    
    head = (struct gate *)malloc(sizeof(struct gate));
    if(head == NULL){
        printf("Error creating final list!!!\n");
        return NULL;
    }
    
    head->gate_name = firstptr->temp_gate_name;
    head->gate_inputs = firstptr->temp_gate_inputs;
    head->gate_output = firstptr->temp_gate_output;
    head->gate_type = firstptr->temp_gate_type;
    head->input = firstptr->input;
    head->output = firstptr->output;
    
    wirehead = (struct wire *)malloc(sizeof(struct wire));
    if(wirehead == NULL){
        printf("Error creating final list->wire!!!\n");
        return NULL;
    }
    
    head->output[0] = wirehead;
    while(head->gate_name[i] != ' '){
        wirehead->name[i] = head->gate_output[i];
        i++;
    }
    wirehead->comingfrom = head;
    
    for(counter=1; counter< head->input; counter++){
        create = (struct wire *)malloc(sizeof(struct wire));
        if(create == NULL){
            printf("Error creating final list->wire!!!\n");
            return NULL;
        }
        head->output[1] = create;
        create->comingfrom = head;
        counterpos=0;
        i++;
        while(head->gate_name[i] != ' '){
            create->name[counterpos] = head->gate_output[i];
            i++;
        }
    }
    
    //Go to next temp_node
    curr = firstptr->next;
    
    while(curr != NULL){
        
        curr = curr->next;
    }
    
    return head;
}*/

struct temp_gate *Find(struct temp_gate *ptr, char stringforsearch[], struct temp_gate *value[]){
    struct temp_gate *node, *curr;
    int counter, i=0;
    
    char *ret;
    for(counter=0; counter<40; counter++){
        value[counter] = NULL;
    }
    
    curr=ptr;
    
    while(curr != NULL){
        ret = strstr(curr->temp_gate_inputs, stringforsearch);
        if(ret != NULL){
            value[i] = curr;
            printf("Found %s\n", ret);
            i++;
        }
        curr = curr->next;
    }
    
    return NULL;
}






