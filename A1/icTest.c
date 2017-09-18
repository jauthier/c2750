

ErrorCode createCalendar(char* fileName){
    
    //check that the file exists and open it
    FILE * fp = fopen(fileName,"r");
    if (fp == NULL){
        return INV_FILE;
    }
    char buffer[75];

    while (fgets(buffer,75,fp) != EOF){
        printf("%s\n", buffer);

        char * token = strtok(buffer, ":;");
        printf("%s\n", token);

        if (strcmp(token, "BEGIN")==0){
            printf("found the start\n");
        }
    }


    return 0;
}

int main(int argc, char * argv[]){
    
    char * fileName = "simpleICFile.ics";
    createCalendar(fileName);
}