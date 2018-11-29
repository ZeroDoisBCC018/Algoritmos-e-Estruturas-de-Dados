int codecheck (int code, LIST* l);

CSV opencsv (char* filename);

int readcsv (CSV fp, LIST* l);

int updatedatabase (CSV fp, char* filename, LIST* l);
