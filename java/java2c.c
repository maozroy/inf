#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EQUALS 0
#define TO_STRING 1
#define HASHCODE 2
#define FINALIZE 3
#define SHOW_COUNTER 4
#define SAY_HELLO 5
#define NUM_OF_MASTERS 6

typedef struct class metadata;
typedef void (*vfunc)(void *);
void *alloc(metadata *data);

struct class
{
    char *name;
    size_t size;
    metadata *super;
    vfunc (*vtable)[];
};

typedef struct 
{
    void *this;
    void *return_val;
    void *obj;
}method_wrapper;

/******************/

typedef struct 
{
    metadata *class;
}object;

void Object_equals(void *param);
void Object_hashCode(void *param);
void Object_toString(void *param);

vfunc object_vtable[3] = {Object_equals, 
                          Object_toString, 
                          Object_hashCode};

metadata object_metadata = {"object", sizeof(object), NULL, &object_vtable};

void Object_equals(void *param)
{
    method_wrapper *wrapper = (method_wrapper *)param;
    int *returnval = (int *)wrapper->return_val;

    (wrapper->this == wrapper->obj) ? *returnval = 1 : 0; 
}

void Object_hashCode(void *param)
{
    method_wrapper *wrapper = (method_wrapper *)param;
    size_t *returnval = (size_t *)wrapper->return_val;

    *returnval = (size_t)(&wrapper->this);
}
 
void Object_toString(void *param)
{
    method_wrapper *wrapper = (method_wrapper *)param;

    Object_hashCode(wrapper);
    sprintf((char *)wrapper->return_val, 
            "%s @ %ld", 
            (char *)((object *)wrapper->this)->class->name, 
            *(size_t *)wrapper->return_val);
}

/******************/
int animal_counter = 0;
int is_first_animal = 1;

typedef struct 
{
    object obj;
    int id;
    int num_legs;
    int num_masters;
}animal;

void animal_Ctor(void *ani);
void animal_toString(void *param);
void animal_sayHello(void *param);
void animal_showCounter();
void animal_finalize(void *param);
void animal_getNumMasters(void *param);
void animal_Ctor_int(animal *ani, int num_of_legs);

vfunc animal_vtable[8] = {Object_equals,
                          animal_toString,  
                          Object_hashCode,
                          animal_finalize, 
                          animal_showCounter, 
                          animal_sayHello,
                          animal_getNumMasters};

metadata animal_metadata = {"animal", sizeof(animal), 
                            &object_metadata, &animal_vtable};

void animal_finalize(void *param)
{
    animal *ani = ((method_wrapper *)param)->this;

    printf("finalize Animal with ID: %d\n", ani->id);
}

void animal_toString(void *param)
{
    method_wrapper *wrapper = (method_wrapper *)param;

    sprintf((char *)wrapper->return_val, 
            "Animal with ID %d", 
            ((animal *)wrapper->this)->id);
}

void animal_sayHello(void *param)
{
    animal *this = param;
    printf("Animal Hello!\n");
    printf("I have %d legs\n", this->num_legs);
}

void animal_showCounter()
{
    printf("%d\n", animal_counter);
}

void animal_getNumMasters(void *param)
{
    method_wrapper *wrapper = (method_wrapper *)param;

    *(int *)wrapper->return_val = ((animal *)wrapper->this)->num_masters;
}

void anima_static_init1()
{
    printf("Static block Animal 1\n");
}

void anima_static_init2()
{
    printf("Static block Animal 2\n");
}

void animal_inst_init()
{
    printf("Instance Initalization block Animal\n");
}

void animal_Ctor_int(animal *ani, int num_of_masters)
{
    printf("Animal Ctor int\n");
    ani->num_masters = num_of_masters;
    ani->id = ++animal_counter;
}

void animal_Ctor(void *ani)
{
    char string[100] = {0};
    method_wrapper wrapper = {NULL};
    animal *new = ani;
    wrapper.return_val = string;
    wrapper.this = ani;

    printf("Animal Ctor\n");
    new->id = ++animal_counter;
    (*new->obj.class->vtable)[SAY_HELLO](&wrapper);
    (*new->obj.class->vtable)[SHOW_COUNTER](&wrapper);

    (*new->obj.class->vtable)[TO_STRING](&wrapper);
    printf("%s\n",(char *) wrapper.return_val);
    Object_toString(&wrapper);
    printf("%s\n",(char *) wrapper.return_val);
}

animal *AnimalCreate(void *space, metadata *data)
{
    animal *ani = (animal *)space;

    if (is_first_animal)
    {
        anima_static_init1();
        anima_static_init2();
        is_first_animal = 0;
    }
    animal_inst_init();

    ani->num_legs = 5;
    ani->num_masters = 1;
    ani->obj.class = data;
    if (strncmp(data->name,"dog", 3) == 0)
    {
        animal_Ctor_int(ani, 2);
    }
    else 
    {
        animal_Ctor(ani);
    }

    return ani;
}

/******************/

void dog_Ctor(void *my_dog);
void dog_toString(void *param);
void dog_sayHello(void *param);
void dog_showCounter();
void dog_finalize(void *param);
int is_first_dog = 1;

typedef struct 
{
    animal animal;
    int num_legs;
}dog;

vfunc dog_vtable[8] = {Object_equals, 
                       dog_toString, 
                       Object_hashCode, 
                       dog_finalize,
                       animal_showCounter,
                       dog_sayHello,
                       animal_getNumMasters};

metadata dog_metadata = {"dog", sizeof(dog), &animal_metadata, &dog_vtable};

void dog_static_init()
{
    printf("Static block Dog 1\n");
}

void dog_inst_init()
{
    printf("Instance initalization block Dog 1\n");
}

void dog_toString(void *param)
{
    method_wrapper *wrapper = (method_wrapper *)param;

    sprintf((char *)wrapper->return_val, 
            "Dog with ID %d\n", 
            ((animal *)wrapper->this)->id);
}

void dog_finalize(void *param)
{
    dog *ani = ((method_wrapper *)param)->this;

    printf("finalize Dog with ID: %d\n", ani->animal.id);
    animal_finalize(param);
}

void dog_sayHello(void *param)
{
    printf("Dog Hello!\n");
    printf("I have %d legs\n", ((animal *)param)->num_legs);
}

void dog_Ctor(void *my_dog)
{
    (void)my_dog;
    printf("Dog Ctor\n");
}

dog *DogCreate(void *space, metadata *data)
{
    dog *doge = (dog *)space;

    if(is_first_dog)
    {
        dog_static_init();
        is_first_dog = 0;
    }
    AnimalCreate(space, data);
    dog_inst_init();
    doge->num_legs = 4;
    dog_Ctor(doge);

    return doge;
}

/******************/

void cat_Ctor(void *my_cat);
void cat_Ctor_color(void *my_cat, char *color);
void cat_toString(void *param);
void cat_finalize(void *param);
int is_first_cat = 1;

typedef struct 
{
    animal animal;
    char colors[10];
    int num_masters;
}cat;

vfunc cat_vtable[8] = {Object_equals, 
                       cat_toString, 
                       Object_hashCode, 
                       cat_finalize,
                       animal_showCounter, 
                       animal_sayHello,
                       animal_getNumMasters};

metadata cat_metadata = {"cat", sizeof(cat), &animal_metadata, &cat_vtable};

void cat_toString(void *param)
{
    method_wrapper *wrapper = (method_wrapper *)param;

    sprintf((char *)wrapper->return_val, 
            "Cat with ID: %d\n", 
            ((cat *)wrapper->this)->animal.id);
}
void cat_finalize(void *param)
{
    cat *ani = ((method_wrapper *)param)->this;
    printf("finalize Cat with ID: %d\n", ani->animal.id);
    animal_finalize(param);
}

void cat_Ctor(void *my_cat)
{
    cat *cato = my_cat;

    cat_Ctor_color(my_cat, "black");
    printf("Cat Ctor\n");
    cato->num_masters = 2;
}

void cat_static_init1()
{
    printf("Static block Cat \n");
}

void cat_Ctor_color(void *my_cat, char *color)
{
    cat *cato = my_cat;

    strcpy(cato->colors, color);
    printf("Cat Ctor with color: %s\n", color);
}

cat *CatCreate(void *space, metadata *data, char *color)
{
    cat *cato = (cat *)space;

    if (is_first_cat)
    {
        cat_static_init1();
        is_first_cat = 0;
    }

    AnimalCreate(space, data);

    cato->num_masters = 5;
    if (NULL == color)
    {
        cat_Ctor(cato);
    }
    else
    {
        cat_Ctor_color(cato, color);
    }

    return cato;
}

/******************/

typedef struct 
{
    cat cat;
}legenderyAnimal;
int is_first_legend = 1;

void legenderyAnimal_toString(void *param);
void legenderyAnimal_finalize(void *param);
void legenderyAnimal_sayHello(void *param);

vfunc legenderyAnimal_vtable[8] = {Object_equals, 
                                  legenderyAnimal_toString, 
                                  Object_hashCode, 
                                  legenderyAnimal_finalize,
                                  animal_showCounter, 
                                  legenderyAnimal_sayHello,
                                  animal_getNumMasters};
metadata legenderyAnimal_metadata = {"legenderyAnimal", sizeof(legenderyAnimal), 
                                    &cat_metadata, &legenderyAnimal_vtable};

void legenderyAnimal_Ctor()
{
    printf("Legendery Ctor\n");
}

void legend_Initalize1()
{
    printf("Static block Legend \n");
}

void legenderyAnimal_toString(void *param)
{
    method_wrapper *wrapper = (method_wrapper *)param;

    sprintf((char *)wrapper->return_val,
            "legenderyAnimal with ID: %d\n", 
            ((legenderyAnimal *)wrapper->this)->cat.animal.id);
}

void legenderyAnimal_finalize(void *param)
{
    legenderyAnimal *ani = ((method_wrapper *)param)->this;

    printf("finalize legenderyAnimal with ID: %d\n", ani->cat.animal.id);
    cat_finalize(param);
}

void legenderyAnimal_sayHello(void *param)
{
    (void) param;
    printf("Legendery Hello!\n");
}
legenderyAnimal *legenderyAnimalCreate(void *space, metadata *data)
{
    legenderyAnimal *legend = (legenderyAnimal *)space;

    if (is_first_legend)
    {
        legend_Initalize1();
        is_first_legend = 0;
    }
    CatCreate(space, data, NULL);
    legenderyAnimal_Ctor();

    return legend;
}

/******************/

void *alloc(metadata *data)
{
    void *space = malloc(data->size);
    
    if (NULL == space)
    {
        exit(0);
    }
    return space;
}

void foo(animal *a)
{
    char string[100] ={0};
    method_wrapper wrapper = {0};
    wrapper.return_val = string;
    wrapper.this = a;
    (*a->obj.class->vtable)[TO_STRING](&wrapper);
    printf("%s", string);
}

int main()
{
    animal *ani = NULL;
    dog *doge = NULL;
    dog *arrdog = NULL;
    cat *cato = NULL;
    cat *arrcat = NULL;
    cat *warrcat = NULL;
    legenderyAnimal *arrla = NULL;
    animal *arrani = NULL;
    legenderyAnimal *shaddad = NULL;
    animal *arr[5] = {0};
    int i = 0;
    int num_of_masters = 0;
    method_wrapper wrapper = {0};

    wrapper.return_val = &num_of_masters;

    ani = alloc(&animal_metadata);
    ani = AnimalCreate(ani, &animal_metadata);

    doge = alloc(&dog_metadata);
    doge = DogCreate(doge, &dog_metadata);

    cato = alloc(&cat_metadata);
    cato = CatCreate(cato, &cat_metadata, NULL);

    shaddad = alloc(&legenderyAnimal_metadata);
    shaddad = legenderyAnimalCreate(shaddad, &legenderyAnimal_metadata);

    animal_showCounter();
    printf("%d\n",ani->id);
    printf("%d\n",((animal *)doge)->id);
    printf("%d\n",((animal *)cato)->id);
    printf("%d\n",((animal *)shaddad)->id);

    arrdog = alloc(&dog_metadata);
    arrdog = DogCreate(arrdog, &dog_metadata);

    arrcat = alloc(&cat_metadata);
    arrcat = CatCreate(arrcat, &cat_metadata, NULL);
        
    warrcat = alloc(&cat_metadata);
    warrcat = CatCreate(warrcat, &cat_metadata, "white");

    arrla = alloc(&legenderyAnimal_metadata);
    arrla = legenderyAnimalCreate(arrla, &legenderyAnimal_metadata);

    arrani = alloc(&animal_metadata);
    arrani = AnimalCreate(arrani, &animal_metadata);

    arr[0] = (animal *)arrdog;
    arr[1] = (animal *)arrcat;
    arr[2] = (animal *)warrcat;
    arr[3] = (animal *)arrla;
    arr[4] = (animal *)arrani;

    for (i = 0; i < 5 ; ++i)
    {
        wrapper.this = arr[i];
        (*arr[i]->obj.class->vtable)[SAY_HELLO](arr[i]);
        (*arr[i]->obj.class->vtable)[NUM_OF_MASTERS](&wrapper);
        printf("%d\n", num_of_masters);
    }

    for (i = 0; i < 5 ; ++i)
    {
        foo(arr[i]);
    }

    free(ani);
    free(doge);
    free(arrdog);
    free(cato);
    free(arrcat);
    free(warrcat);
    free(arrla);
    free(arrani);
    free(shaddad);

    return 0;
}