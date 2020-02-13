/**********************************************************************
 *	This code is representing my implementation of Java2C RC2         *
 *	Lab: FS-789                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: David Berman     	  				    		     	  *
 *	Date: 12.02.20 													  *
 **********************************************************************/
#include <stdio.h>  /*printf*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*strncpy*/

#define MAX_STRING_SIZE 1000
#define UNUSE(x) (void)(x)

enum vFunctions
{
    TO_STRING,
    EQUALS,
    FINALIZE,
    GET_CLASS,
    HASHCODE,
    SAY_HELLO,
    GET_NUM_MASTERS
};

enum boolean
{
    FALSE,
    TRUE
};

typedef struct class metadata;
typedef void (*vfunc)(void*);

typedef struct
{
    metadata *class;
}Object;

struct class
{
    char *name;
    size_t size;
    metadata *super;
    vfunc(*vtable)[];
};/*metadata*/

typedef struct
{
    char string[MAX_STRING_SIZE];
    int result;

    Object *this;
    Object *other;
}wrapper_t;

typedef struct 
{
    Object obj;
    
    int num_legs;
    int counter;
    int num_masters;
    int ID;
}Animal;

typedef struct 
{
    Animal animal;
    int num_legs;
    /*ID - inherited from Animal*/
}Dog;

typedef struct 
{
    Animal animal;
    int num_masters;
    char colors[MAX_STRING_SIZE];
    /*ID - inherited from Animal*/
}Cat;

typedef struct 
{
    Cat cat;
    /*num_masters - inherited from Cat*/
    /*colors - inherited from Cat*/
    /*ID - inherited from Animal*/
}LegendaryAnimal;

int first_time_animal = TRUE;
int first_time_dog = TRUE;
int first_time_cat = TRUE;
int first_time_la = TRUE;
int counter = 0;

void foo(Animal *a);
void showCounter();
void Object_ctor_no_arg();
void Object_equals(void *param);
void Object_finalize(void *param);
void Object_getClass(void *param);
void Object_hashCode(void *param);
void Object_toString(void *param);
void LoadAnimalStatic();
void NewAnimal_no_arg(Animal **animal ,metadata* metadata);
void CreateAnimal_arg(Animal *animal, int num_masters);
void CreateAnimal_no_arg(Animal *animal);
void Animal_static_block1();
void Animal_static_block2();
Animal *Animal_ctor_no_arg(Animal *animal);
Animal *Animal_ctor_arg(Animal *animal, int num_masters);
void Animal_toString(void *param);
void Animal_finalize(void *param);
void Animal_sayHello(void *param);
void Animal_getNumMasters(void *param);
void LoadDogStatic();
void NewDog_no_arg(Dog **dog ,metadata* metadata);
void CreateDog_no_arg(Dog *dog);
void Dog_static_block();
Dog *Dog_ctor_no_arg(Dog *dog);
void Dog_toString(void *param);
void Dog_finalize(void *param);
void Dog_sayHello(void *param);
void LoadCatStatic();
void NewCat_no_arg(Cat **cat ,metadata* metadata);
void NewCat_arg(Cat **cat ,char* colors ,metadata* metadata);
void CreateCat_arg(Cat *cat, char *colors);
void CreateCat_no_arg(Cat *cat);
void Cat_static_block();
Cat *Cat_ctor_arg(Cat *cat, char *colors);
Cat *Cat_ctor_no_arg(Cat *cat);
void Cat_toString(void *param);
void Cat_finalize(void *param);
void LoadLegendaryAnimalStatic();
void NewLegendaryAnimal_no_arg(LegendaryAnimal **la ,metadata* metadata);
void CreateLegendaryAnimal_no_arg(LegendaryAnimal *la);
void LegendaryAnimal_static_block();
LegendaryAnimal *LegendaryAnimal_ctor_no_arg(LegendaryAnimal *la);
void LegendaryAnimal_toString(void *param);
void LegendaryAnimal_finalize(void *param);
void LegendaryAnimal_sayHello(void *param);

/***************************Initializing classes*****************************/

vfunc Object_vtable[] = {Object_toString, Object_equals, Object_finalize,
                         Object_getClass, Object_hashCode};
metadata Object_metadata = {"Object", sizeof(Object), NULL, &Object_vtable};

vfunc Animal_vtable[] = {Animal_toString, Object_equals, Animal_finalize,
                         Object_getClass, Object_hashCode, Animal_sayHello,
                         Animal_getNumMasters};
metadata Animal_metadata = 
                {"Animal", sizeof(Animal), &Object_metadata, &Animal_vtable};

vfunc Dog_vtable[] = {Dog_toString, Object_equals, Dog_finalize,
                      Object_getClass, Object_hashCode, Dog_sayHello,
                      Animal_getNumMasters};
metadata Dog_metadata = {"Dog", sizeof(Dog), &Animal_metadata, &Dog_vtable};


vfunc Cat_vtable[] = {Cat_toString, Object_equals, Cat_finalize,
                      Object_getClass, Object_hashCode, Animal_sayHello,
                      Animal_getNumMasters};
metadata Cat_metadata = {"Cat", sizeof(Cat), &Animal_metadata, &Cat_vtable};

vfunc LegendaryAnimal_vtable[] = 
            {LegendaryAnimal_toString, Object_equals, LegendaryAnimal_finalize,
             Object_getClass, Object_hashCode, LegendaryAnimal_sayHello,
             Animal_getNumMasters};
metadata LegendaryAnimal_metadata = 
{"LegendaryAnimal", sizeof(LegendaryAnimal), &Cat_metadata, &LegendaryAnimal_vtable};

int main()
{
    size_t i = 0;

    Animal *animal1 = NULL;
    Dog *dog1 = NULL;
    Cat *cat1 = NULL;
    LegendaryAnimal *la1 = NULL;
    Animal *array[5] = {0};

    Animal *animal2 = NULL;
    Dog *dog2 = NULL;
    Cat *cat2 = NULL;
    Cat *cat2_white = NULL;
    LegendaryAnimal *la2 = NULL;

    wrapper_t wrapper;

    NewAnimal_no_arg(&animal1 ,&Animal_metadata);
    NewDog_no_arg(&dog1 ,&Dog_metadata);
    NewCat_no_arg(&cat1 ,&Cat_metadata);
    NewLegendaryAnimal_no_arg(&la1 ,&LegendaryAnimal_metadata);

    showCounter();
    printf("%d\n",animal1->ID);
    printf("%d\n",dog1->animal.ID);
    printf("%d\n",cat1->animal.ID);
    printf("%d\n",la1->cat.animal.ID);
    
    NewDog_no_arg(&dog2 ,&Dog_metadata);
    NewCat_no_arg(&cat2 ,&Cat_metadata);
    NewCat_arg(&cat2_white, "white" ,&Cat_metadata);
    NewLegendaryAnimal_no_arg(&la2 ,&LegendaryAnimal_metadata);
    NewAnimal_no_arg(&animal2 ,&Animal_metadata);

    array[0] = (Animal *)dog2;
    array[1] = (Animal *)cat2;
    array[2] = (Animal *)cat2_white;
    array[3] = (Animal *)la2;
    array[4] = (Animal *)animal2;

    for(i = 0; i < 5; ++i)
    {
        wrapper_t wrapper;
        wrapper.this = (void *)array[i];
        
        (*array[i]->obj.class->vtable)[SAY_HELLO](&wrapper);
        Animal_getNumMasters(&wrapper);
        printf("%d\n",wrapper.result);
    }
    
    for(i = 0; i < 5; ++i)
    {
        foo(array[i]);
    }

    printf("After gc\n\n");

    printf("\n***Finalizing animal1***\n");
    wrapper.this = (void *)animal1;
    (*animal1->obj.class->vtable)[FINALIZE](&wrapper);

    printf("\n***Finalizing dog1***\n");
    wrapper.this = (void *)dog1;
    (*dog1->animal.obj.class->vtable)[FINALIZE](&wrapper);

    printf("\n***Finalizing cat1***\n");
    wrapper.this = (void *)cat1;
    (*cat1->animal.obj.class->vtable)[FINALIZE](&wrapper);

    printf("\n***Finalizing la1***\n");
    wrapper.this = (void *)la1;
    (*la1->cat.animal.obj.class->vtable)[FINALIZE](&wrapper);

    printf("\n***Finalizing animal2***\n");
    wrapper.this = (void *)animal2;
    (*animal2->obj.class->vtable)[FINALIZE](&wrapper);

    printf("\n***Finalizing dog2***\n");
    wrapper.this = (void *)dog2;
    (*dog2->animal.obj.class->vtable)[FINALIZE](&wrapper);

    printf("\n***Finalizing cat2***\n");
    wrapper.this = (void *)cat2;
    (*cat2->animal.obj.class->vtable)[FINALIZE](&wrapper);

    printf("\n***Finalizing cat2_white***\n");
    wrapper.this = (void *)cat2_white;
    (*cat2_white->animal.obj.class->vtable)[FINALIZE](&wrapper);

    printf("\n***Finalizing la2***\n");
    wrapper.this = (void *)la2;
    (*la2->cat.animal.obj.class->vtable)[FINALIZE](&wrapper);

    for(i = 0; i < 5; ++i)
    {
        free(array[i]);
    }
    free(animal1);
    free(dog1);
    free(cat1);
    free(la1);

    return 0;
}

void foo(Animal *a)
{
    wrapper_t wrapper;
    wrapper.this = (void *)a;
    (*(a->obj.class)->vtable)[TO_STRING](&wrapper);
    printf("%s\n", wrapper.string);         /*printing toString output*/
}

void showCounter()
{
    printf("%d\n", counter);
}

/***************************Object*****************************/

void Object_ctor_no_arg(){}

void Object_equals(void *param)
{
    int is_equal = (((wrapper_t *)param)->this == 
                     ((wrapper_t *)param)->other);
    
    ((wrapper_t *)param)->result = is_equal;                
    printf("Object_equals\n");
}

void Object_finalize(void *param) 
{
    UNUSE(param);
    printf("Object_finalize\n");
}
void Object_getClass(void *param) 
{
    UNUSE(param);
    printf("Object_getClass\n");
}
void Object_hashCode(void *param) 
{
    UNUSE(param);
    printf("Object_hashCode\n");
}

void Object_toString(void *param)
{
    char *calling_class_name = 
                      ((metadata *)(((wrapper_t *)param)->this)->class)->name;
    sprintf(((wrapper_t *)param)->string, "%s@101010101", calling_class_name);
}

/***************************Animal*****************************/

void LoadAnimalStatic()
{
    if(first_time_animal)
    {
        Animal_static_block1();
        Animal_static_block2();
        first_time_animal = FALSE;
    }
}

void NewAnimal_no_arg(Animal **animal ,metadata* metadata)
{
    LoadAnimalStatic();
    *animal = (Animal *)malloc((*metadata).size);
    if(NULL == *animal)
    {
        exit(1);
    }
    (*animal)->obj.class = metadata;
    CreateAnimal_no_arg(*animal);
}

void CreateAnimal_arg(Animal *animal, int num_masters)
{
    Object_ctor_no_arg();
    printf("Instance initialization block Animal\n");
    animal->num_legs = 5;
    Animal_ctor_arg(animal, num_masters);
}

void CreateAnimal_no_arg(Animal *animal)
{
    Object_ctor_no_arg();
    printf("Instance initialization block Animal\n");
    animal->num_legs = 5;
    animal->num_masters = 1;
    Animal_ctor_no_arg(animal);
}

void Animal_static_block1()
{
    printf("Static block Animal 1\n");
}
void Animal_static_block2()
{
    printf("Static block Animal 2\n");
}

Animal *Animal_ctor_no_arg(Animal *animal)
{   
    wrapper_t wrapper = {0};
    wrapper.this = (void *)animal;
    animal->ID = ++counter;
    printf("Animal Ctor\n");
    (*(animal->obj.class)->vtable)[SAY_HELLO](&wrapper);
    showCounter();
    (*(animal->obj.class)->vtable)[TO_STRING](&wrapper);
    printf("%s\n", wrapper.string);         /*printing toString output*/
    Object_toString(&wrapper);
    printf("%s\n", wrapper.string);         /*printing super.toString output*/ 

    return animal;
}

Animal *Animal_ctor_arg(Animal *animal, int num_masters)
{
    printf("Animal Ctor int\n");
    animal->ID = ++counter;
    animal->num_masters = num_masters;

    return animal;
}

void Animal_toString(void *param)
{
    int ID = ((Animal *)((wrapper_t *)param)->this)->ID;
    sprintf(((wrapper_t *)param)->string, "Animal with ID: %d", ID);
}

void Animal_finalize(void *param)
{
    printf("finalize Animal with ID: %d\n", 
                        ((Animal *)((wrapper_t *)param)->this)->ID);
    Object_finalize(param);
}

void Animal_sayHello(void *param)
{
    printf("Animal Hello!\n");
    printf("I have %d legs\n", ((Animal *)((wrapper_t *)param)->this)->num_legs);
}

void Animal_getNumMasters(void *param)
{
   ((wrapper_t *)param)->result = 
                ((Animal *)((wrapper_t *)param)->this)->num_masters;
}

/***************************Dog*****************************/

void LoadDogStatic()
{
    if(first_time_dog)
    {
        Dog_static_block();
        first_time_dog = FALSE;
    }
}

void NewDog_no_arg(Dog **dog ,metadata* metadata)
{
    LoadAnimalStatic();
    LoadDogStatic();
    *dog = (Dog *)malloc((*metadata).size);
    if(NULL == *dog)
    {
        exit(1);
    }
    ((*dog)->animal).obj.class = &Dog_metadata;
    CreateDog_no_arg(*dog);
}

void CreateDog_no_arg(Dog *dog)
{
    CreateAnimal_arg(&(dog->animal), 2);
    printf("Instance initialization block Dog\n");
    dog->num_legs = 4;
    Dog_ctor_no_arg(dog);
}

void Dog_static_block()
{
    printf("Static block Dog\n");
}

Dog *Dog_ctor_no_arg(Dog *dog)
{
    printf("Dog Ctor\n");

    return dog;
}

void Dog_toString(void *param)
{
    int ID = ((Dog *)((wrapper_t *)param)->this)->animal.ID;
    sprintf(((wrapper_t *)param)->string, "Dog with ID: %d", ID);
}

void Dog_finalize(void *param)
{
    printf("finalize Dog with ID: %d\n", 
                              ((Dog *)((wrapper_t *)param)->this)->animal.ID);
    Animal_finalize(param);
}

void Dog_sayHello(void *param)
{
    printf("Dog Hello!\n");
    printf("I have %d legs\n", ((Dog *)((wrapper_t *)param)->this)->num_legs);
}

/***************************Cat*****************************/

void LoadCatStatic()
{
    if(first_time_cat)
    {
        Cat_static_block();
        first_time_cat = FALSE;
    }
}

void NewCat_no_arg(Cat **cat ,metadata* metadata)
{
    LoadAnimalStatic();
    LoadCatStatic();
    *cat = (Cat *)malloc((*metadata).size);
    if(NULL == *cat)
    {
        exit(1);
    }
    ((*cat)->animal).obj.class = &Cat_metadata;
    CreateCat_no_arg(*cat);
}

void NewCat_arg(Cat **cat ,char* colors ,metadata* metadata)
{
    LoadAnimalStatic();
    LoadCatStatic();
    *cat = (Cat *)malloc((*metadata).size);
    if(NULL == *cat)
    {
        exit(1);
    }
    ((*cat)->animal).obj.class = &Cat_metadata;
    CreateCat_arg(*cat, colors);
}

void CreateCat_arg(Cat *cat, char *colors)
{
    CreateAnimal_no_arg((&cat->animal));
    cat->num_masters = 5;
    Cat_ctor_arg(cat, colors);
}

void CreateCat_no_arg(Cat *cat)
{
    CreateAnimal_no_arg((&cat->animal));
    cat->num_masters = 5;
    Cat_ctor_no_arg(cat);
}

void Cat_static_block()
{
    printf("Static block Cat\n");
}

Cat *Cat_ctor_arg(Cat *cat, char *colors)
{   
    strcpy(cat->colors, colors);
    printf("Cat Ctor with color: %s\n", cat->colors);

    return cat;
}

Cat *Cat_ctor_no_arg(Cat *cat)
{
    Cat_ctor_arg(cat, "black");
    printf("Cat Ctor\n");
    cat->num_masters = 2;

    return cat;
}

void Cat_toString(void *param)
{
    int ID = ((Cat *)((wrapper_t *)param)->this)->animal.ID;
    sprintf(((wrapper_t *)param)->string, "Cat with ID: %d", ID);
}

void Cat_finalize(void *param)
{
    printf("finalize Cat with ID: %d\n", 
                              ((Cat *)((wrapper_t *)param)->this)->animal.ID);
    Animal_finalize(param);
}

/***************************LegendaryAnimal*****************************/

void LoadLegendaryAnimalStatic()
{
    if(first_time_la)
    {
        LegendaryAnimal_static_block();
        first_time_la = FALSE;
    }
}

void NewLegendaryAnimal_no_arg(LegendaryAnimal **la ,metadata* metadata)
{
    LoadAnimalStatic();
    LoadCatStatic();
    LoadLegendaryAnimalStatic();
    *la = (LegendaryAnimal *)malloc((*metadata).size);
    if(NULL == *la)
    {
        exit(1);
    }
    ((*la)->cat).animal.obj.class = &LegendaryAnimal_metadata;
    CreateLegendaryAnimal_no_arg(*la);
}

void CreateLegendaryAnimal_no_arg(LegendaryAnimal *la)
{
    CreateCat_no_arg((&la->cat));
    LegendaryAnimal_ctor_no_arg(la);
}

void LegendaryAnimal_static_block()
{
    printf("Static block LegendaryAnimal\n");
}

LegendaryAnimal *LegendaryAnimal_ctor_no_arg(LegendaryAnimal *la)
{
    printf("Legendary Ctor\n");

    return la;
}

void LegendaryAnimal_toString(void *param)
{
    int ID = ((LegendaryAnimal *)((wrapper_t *)param)->this)->cat.animal.ID;
    sprintf(((wrapper_t *)param)->string, "LegendaryAnimal with ID: %d", ID);
}

void LegendaryAnimal_finalize(void *param)
{
    printf("finalize LegendaryAnimal with ID: %d\n", 
              ((LegendaryAnimal *)((wrapper_t *)param)->this)->cat.animal.ID);
    Cat_finalize(param);
}

void LegendaryAnimal_sayHello(void *param)
{
    UNUSE(param);
    printf("Legendary Hello!\n");
}