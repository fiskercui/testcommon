#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>


class Graphic
{
public:
    virtual ~Graphic(){

    }

    virtual  void Draw(){

    }

protected:
    Graphic(){
    }
};


class Image: public Graphic
{
public:
    Image(const char* szImage){
        m_szName = strdup(szImage);
    }
    virtual ~Image() {

    }

    virtual void Draw(){
        std::cout << "Image Draw" << std::endl;
    }

    //   virtual void FromStream()
private:
    char* m_szName;
};

class ImageProxy: public Graphic
{
public:
    ImageProxy(const char* szImage){
        m_szFileName = strdup(szImage);
//        m_image =new Image(szImage);
        m_image = 0;
    }
    virtual ~ImageProxy(){

    }
    virtual void Draw(){
        m_image = new Image(m_szFileName);
        GetImage()->Draw();
    }
protected:
    Image* GetImage(){
        return m_image;
    }

private:
    Image* m_image;
    char* m_szFileName;
};


int main(int argc, char *argv[])
{
    ImageProxy* pIp = new ImageProxy("abc");
    pIp->Draw();
    return 0;
}
