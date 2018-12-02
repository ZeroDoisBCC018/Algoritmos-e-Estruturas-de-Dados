void update_database (CSV csv, char* name, LIST* list){
	
	int i = 0;
	SITE* percorre;
	
	percorre = list->first;
	
	freopen(name, "w", csv);
	
	fprintf(csv, "\"Index\",\"Site\",\"Relevance\",\"Link\",\"Tags\"");
	fprintf(csv, "\n");
	
	while (percorre != NULL)
	{
	
	fprintf(csv, "%d,%s,%d,%s", percorre->code, percorre->name, percorre->rel, percorre->link);
	fprintf(csv, "\"");
	
		while (percorre->tag[i][0] != '\0' || i <= 10)
		{
			fprintf(csv, "%s", percorre->tag[i]);
			i++;
			
			if(percorre->tag[i+1][0] != '\0') printf (",");
		}
		
		fprintf(csv, "\"");
		fprintf(csv, "\n");
	
	percorre = percorre->next;
	
	}

}
