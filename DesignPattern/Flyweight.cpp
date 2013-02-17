#include <stdio.h>
#include <unistd.h>




class Glyph
{
public:
    Glyph(){

    }
    virtual ~Glyph(){

    }
};

class Character: public Glyph
{
public:
    Character(char c){

    }
    virtual ~Character(){

    }
};


const int NCCHARCODES = 128;


class GlyphFactory
{
public:
    GlyphFactory(){
        for(int i =0; i< NCCHARCODES; i++)
        {
            m_character[i] = NULL;
        }
    }
    virtual ~GlyphFactory(){
        for(int i=0; i<NCCHARCODES; i++){
            if(m_character[i] != NULL){
                delete m_character[i];
                m_character[i] = NULL;
            }
        }
    }

    virtual Character* CreateCharacter(char c){
        if(m_character[c] == NULL)
        {
            m_character[c] = new Character(c);
        }
        return m_character[c];
    }
//    virtual Row* CreateRow();
//

private:
    Character* m_character[NCCHARCODES];
};


GlyphFactory* GetGlyphFactory(){
    static GlyphFactory s_factory;
    return &s_factory;
}

int main(int argc, char *argv[])
{
    GetGlyphFactory()->CreateCharacter('a');
    GetGlyphFactory()->CreateCharacter('b');
    return 0;
}
