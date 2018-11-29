
int codecheck (int code, LIST* l){
	if (invalidlist(l)) return ERROR;
	if (emptylist (l)) return ERROR;
	if (code < 0 || code > 9999) return ERROR;
	SITE* aux = l->first;
	while (aux != NULL){
		if (aux->code == code) return ERROR;
		else aux = aux->next;
	}
	return SUCCESS;
}

CSV opencsv (char* filename){
	if(filename == NULL){
		perror(".csv filename not given.\n");
		exit(EXIT_FAILURE);
	}
	CSV fp;
	fp = fopen(filename, "r");
	if (DEBUG) printf("PONTEIRO foi aberto: %p\n", fp);
	if(fp == NULL){
		perror(".csv file could not be opened. Not enough memory/fatal error ocurred while trying to access the disk.\n");
		exit(EXIT_FAILURE);
	}
	return fp;
}


int readcsv (CSV fp, LIST* l){
	if (invalidlist(l)) return ERROR;
	int i = 0, j = 0, k = 0;
	char dump = ' '; /* Dump is initialized as a blank space so it certainly enters the reading loop on line x. */
	if(fp == NULL){
		perror("File not opened.\n");
		return ERROR;
	}
	
	rewind(fp);
	
	char ignoreline[200];
	fscanf(" %[^\n\r]%*c", ignoreline, fp); /* Ignores the header from the CSV file. */
	int count1 = 0;
	int count2 = 0;
	
	do{ 
		fscanf(" %[^\n\r]%*c", ignoreline, fp);
		count1++;
		if (DEBUG) printf("count1 = %d\n", count1);
	} while(!feof(fp)) /* This loop counts all the lines of the csv file and rewinds it before performing the reading operations. */
	
	rewind(fp);
	
	SITE* n = NULL;
	n = newsite(); /* In this loop a number of nodes equivalent to the number of sites counted are created and linked. */
	l->first = n;
	 SITE* aux = NULL;
	 aux = n;
	 
	while(count2 != (count1-1)){
		aux->next = newsite();
		aux = n->next;
		count2++;
		if(DEBUG) printf("count2 = %d\n", count2);
	}
	printf("saiu do loop count2\n");
	 
	aux = l->first; /* Now the node is reset to the first position of the list in order to read the informations. */
	int count3 = 0;
	
	do{
		fscanf(" %[^\n\r]%*c", ignoreline, fp); /* Ignores again the header. */
		if(DEBUG) printf("header ignorado wahoo\n");
		
		fscanf(fp, "%d", &aux->code);
		
		if (DEBUG) printf("code lido = %d\n", aux->site->code);
		fscanf(fp, ",%[^,],%d,%[^,],", aux->name, &aux->rel, aux->link);
		if (DEBUG) printf("nome, relevancia e link lidos\n");
		fgetc(fp); /*ignora aspas iniciais do link*/
		if (DEBUG) printf("Antes de zerar colunas das tags\n");

		for(i = 0; i < 10; i++){
			aux->tag[i][0] = '#';
		}
		
		printf("Colunas de tag zeradas\n");
		
		while (dump != '"'){
			dump = fgetc(fp);
			if(dump == '"') break;
			if(dump == ','){
				 j++;
				 k = 0;
				 dump = fgetc(fp);
			}
			else{
				aux->site->tag[j][k] = dump;
				k++;
			}
		}	
		
		if (aux->next == NULL) break;
		aux = n->next;
		l->size++;
		count3++;
		printf("do while realizado %d vezes\n", count3);
	}while(!feof(fp));
	printf("linha final do csv, linhas do test foram lidas\n");
	return SUCCESS;
}

int updatedatabase (CSV fp, char* filename, LIST* l) {
	if (fp == NULL){
		perror("File not opened.\n");
		return ERROR;
	}
	
	freopen(filename, "w", CSV);
	
	if (fp == NULL){
		perror("An error ocurred while overwriting the file.\n");
		return ERROR;
	}
	
	int i = 0;
	int j = 0;
	
	SITE* aux;
	aux = l->first;
	char c = ' ';
	
	printf("\"Index\",\"Site\",\"Relevance\",\"Link\",\"Tags\"\n");
	
	while (aux->next != l->last){
		fprintf(fp, "%04d,%s,%s", aux->site->code, aux->site->name, aux->site->link);
		do{
			for(i = 0; i < 10; i++){
				fprintf(fp, ",");
				for(j = 0; j < 50; j++){
					fprintf(fp, "%c", aux->site->tag[i][j]);
				}
			}
			printf("\n");
		}while (c != '\0');
		aux = aux->next;
	}
	fclose(fp);
	return SUCCESS;
}
