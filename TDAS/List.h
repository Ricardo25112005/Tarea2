#ifndef List_h
#define List_h

typedef struct List List;

List * create_List(void);

void * list_firts(List * list);

void * list_next(List *list);

void * list_prev(List *list);

void * push_back(List * list, void * ticket);

void * push_front(List * list, void * ticket);

void * push_current(List * list, void * ticket);

void * pop_front(List * list);

void * pop_back(List * list);

void * pop_current(List * list);   

void cleanList(List * list);

#endif /* List_h */