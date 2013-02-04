#include <stdio.h>


enum eProductId
{
    eMine,
    eYours,
    eTheirs,
};

class Product
{
public:
    Product(){
        printf("create common product\n");
    }
    virtual ~Product(){

    }
};


class MyProduct: public Product
{
public:
    MyProduct(){
        printf("create my product\n");
    }
    virtual ~MyProduct(){

    }
};

class YourProduct:public Product
{
public:
    YourProduct(){
        printf("create your product\n");
    }
    virtual ~YourProduct(){

    }
};

class TheirProduct:public Product
{
public:
    TheirProduct(){
        printf("create their product\n");
    }
    virtual ~TheirProduct(){

    }
};


class Creator
{
public:
    Creator(){
    }

    virtual Product* Create(int id){
        printf("Creat product\n");
        if(id == eMine)
            return new MyProduct;
        else if(id == eYours)
            return new YourProduct;
        else
            return new Product;
    }
    virtual ~Creator(){

    }
};

class MyCreator
{
public:
    MyCreator(){

    }
    virtual Product* Create(int id){
        printf("MyCreate product\n");
        if(id == eMine)
            return new YourProduct;
        else if(id == eYours)
            return new MyProduct;
        else if(id == eTheirs)
            return new TheirProduct;
        else
            return new Product;
    }
    virtual ~MyCreator(){

    }
};


//not construct Creator SubClass

class Creator2{
public :
    virtual Product* CreateProduct() = 0;
};

template <typename TheProduct>
class StandardCreator: public Creator2{
public :
    virtual Product* CreateProduct();
};

template<typename TheProduct>
Product* StandardCreator<TheProduct>::CreateProduct(){
    return new TheProduct;
}


int main(int argc, char *argv[])
{
    Creator creator;
    creator.Create(1);

    MyCreator mycreator;
    mycreator.Create(1);

    StandardCreator<MyProduct> myCreator;
    myCreator.CreateProduct();
    return 0;
}

