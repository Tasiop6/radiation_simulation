    
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
            tnode->input = FindInputOutputs(tnode->gate_inputs);
            tnode->output = FindInputOutputs(tnode->gate_output);
            return 0;
            //continue;
        }
        //loop to Create to wire list?
        //and nodes
        int create=0, create1=0;
        struct wire *next, *prev;
        while(create < tnode->output){
            next = (struct wire *)malloc(sizeof(struct wire));
            if(next == NULL){
                printf("Error alocating memmory for the wire!!!\nExiting...\n");
                return -1;
            }
            tnode->next[create] = next;
            create++;
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
