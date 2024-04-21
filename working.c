/*****************************************************/
/*                                                   */
/* Created By Konstantinos Tasiopoulos               */
/* AEM: 2600                                         */
/* Sxediasmos kiklwmatwn gia iontizonta perivalonta  */
/*                                                   */
/*****************************************************/ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 512

struct gate{
    char gate_name[50];
    char gate_inputs[200];
    char gate_output[100];
    char gate_type[50];
    struct gate *next;
    //I will point to node n
};


//For wires
struct node{
    struct gate *wire1;
    struct gate *wire2;
    struct gate *wire3;
    struct gate *wire4;
    struct gate *wire5;
};

//Funtrions
//Function to set \0 string
void SetZero(char str[], int size);
void BasicDataSpliter(struct gate *ptr, char tosplit[]);
void InputOutputSpliter(struct gate *ptr, char tosplit[]);
char * StringSplit(char tosplit[]);

//Free pointers
void FreeMem(struct gate **ptr);

int main(int argc, char *argv[]){
    
    //File pointer 
    FILE *fp;
    char str[SIZE]; //String to read from file
    
    //String for the circuit input - output - wire
    char input[200];
    char output[50];
    char wire[200];
    char temp[50];
    char readtemp[600];
    char read[600];
    
    //Which line from file i read starting from input(0)
    int counter, lineread = 0; 
    int flag=0;
    
    //Head of temp linked list
    struct gate *head = NULL;
    struct gate *tnode;
    struct gate *curr;
    
    //Initialize everything to \0
    SetZero(str, SIZE);
    SetZero(input, 200);
    SetZero(output, 50);
    SetZero(wire, 200);
    SetZero(temp,50);
    SetZero(readtemp, 600);
    SetZero(read, 600);
    
    // opening file for reading
    fp = fopen("s27_mapped.v" , "r");
    if(fp == NULL) {
       perror("Error opening file");
      
      return(-1);
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
            strcpy(input, read);
            SetZero(str, SIZE);
            SetZero(readtemp, 600);
            SetZero(read, 600);
            lineread++;
            continue;
        }
        //Read output
        if(lineread == 1){
            strcpy(output, read);
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
        //Create Gate Structs frome here
        //Just building the list as given from file
        //I will rebuild it later
        
        //Drop empty lines no line will have length less than 2
        if(strlen(read) < 2){
            lineread++;
            continue;
        }
        
        //lineread is 4 means i just started reading the gates
        if(lineread == 4){
            head = (struct gate *)malloc(sizeof(struct gate));
            if(head == NULL){
                printf("Error alocating memmory for the gate!!!\nExiting...\n");
                return -1;
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
        curr = (struct gate *)malloc(sizeof(struct gate));
        if(curr == NULL){
            printf("Error alocating memmory for the gate!!!\nExiting...\n");
            return -1;
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
    
    //Close pointer to file
    fclose(fp);
    
    //Print check
    //puts(input);
    //puts(output);
    //puts(wire);
    
    
    curr=head;
    while(curr!=NULL){
       printf("%s\n%s\n%s\n%s\n", curr->gate_name, curr->gate_inputs, curr->gate_output, curr->gate_type);
       printf("\n"); 
       curr = curr->next;
    }
    
    //Free the list
    FreeMem(&head);
    
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

void BasicDataSpliter(struct gate *ptr, char tosplit[]){
    char temp[100];
    int counter=0, counter1=0;
    
    SetZero(temp,100);
    
    //First get the name
    while(tosplit[counter] != '('){
        temp[counter] = tosplit[counter];
        counter++;
    }
    //Delete last space from gate name
    temp[counter] = '\0';
    while(counter1 < counter){
        temp[counter1] = temp[counter1+2];
        counter1++;
    }
    
    //Set it to the gate_name
    strcpy(ptr->gate_name, temp);
    
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

void InputOutputSpliter(struct gate *ptr, char tosplit[]){
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
    printf("%s\n", temp);
    for(counter=0; counter<counter1; counter++){
        temp[counter] = temp[counter+1];
    }
    char output[200];
    SetZero(output, 200);
    strcpy(output, temp);
    strcat(output, ";");
    strcpy(ptr->gate_output, StringSplit(output));
    
    SetZero(temp, 100);
    for(counter=0; counter<inp; counter++){
        temp[counter] = tosplit[counter];
    }
    //Drop space
    for(counter=0; counter<inp; counter++){
        temp[counter] = temp[counter+1];
    }
    strcat(temp,";");
    printf("%s\n", temp);
    strcpy(ptr->gate_inputs, StringSplit(temp));
    
    //Setting type
    char type[30];
    strcpy(type, ptr->gate_name);
    //You have 2 spaces before a letter
    if(type[0] == 'D'){
        strcpy(ptr->gate_type ,"D_Flif_Flop");
    }
    else if(type[0] == 'O'){
        strcpy(ptr->gate_type, "OR");
    }
    else if(type[0] == 'I'){
        strcpy(ptr->gate_type, "Inverter");
    }
    else if(type[0] == 'A'){
        strcpy(ptr->gate_type, "AND");
    }
    else if(type[0] == 'N'){
        if(type[1] == 'A'){
            strcpy(ptr->gate_type, "NAND");
        }
        else if(type[1] == 'O'){
            strcpy(ptr->gate_type, "NOR");
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
void FreeMem(struct gate **ptr){
    struct gate *curr;
    struct gate *next;
    
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

