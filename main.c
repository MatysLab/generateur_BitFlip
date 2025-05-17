
#include "oper_bits.c"
#include <assert.h>
#include "gabarit.c"


#define TEST 0

#define N 32

// Declaration des fonctions //

int isContigue(unsigned int etats_generateurr);
unsigned int get_bits_dispo(unsigned int etat_bits);
int choix_alea_bit1(unsigned int etat_bits);
unsigned int initGenerateur( void );
unsigned int permuter_bits( unsigned int etat_gen_ions );

void testIsContigue(void);
void testGet_bits_dispo(void);
void testChoix_alea_bit1(void);


//************************Definition des fonctions************************//
int isContigue(unsigned int etats_generateurr)
{
    // Si un des generateurs est contigus, return index
    for (unsigned int index = 0, count = 0; index < 32; ++index)
    {
        if (get_bit(etats_generateurr, index))
            ++count;
        else
            count = 0;

        if (count == 3)
            return 1;
    }
    // Si aucun generateurs est contigus, return -1
    return 0;
}

unsigned int get_bits_dispo(unsigned int etat_bits)
{
    unsigned int bits_possibles = 0;

    if (etat_bits == 0)
        return 0xFFFFFFFF;
    else if (etat_bits == 0xFFFFFFFF)
        return 0;

    for (int i = 0; i < N; ++i)
    {
        if (!get_bit(etat_bits, i) && !isContigue(etat_bits + (1 << i)))
        {
            bits_possibles = set_bit(bits_possibles, i);
        }
    }

    return bits_possibles;
}

int choix_alea_bit1(unsigned int etat_bits)
{
    if (!etat_bits)
        return -1;

    unsigned int index = 0;
    do
    {
        index = randi(N) - 1;
    } while (!get_bit(etat_bits, index));

    return index;
}

unsigned int initGenerateur( void )
{
    unsigned int config_initiale = 0;
    unsigned int bits_dispo = get_bits_dispo ( config_initiale );
    unsigned int ordre = 0;

    unsigned int k = 0;

    while ( bits_dispo && ( k < (0.28*N) || k <= (0.48*N) ) )
    {
        ordre = choix_alea_bit1( bits_dispo );
        assert ( ordre >= 0 && ordre < N );
        config_initiale += (1 << ordre);
        ++k;

        bits_dispo = get_bits_dispo ( config_initiale );
    }
    assert ( !isContigue ( config_initiale ) );

    return config_initiale;
}

unsigned int permuter_bits( unsigned int etat_gen_ions )
{
    //Choisir un bit disponible 0
    int posInactive = choix_alea_bit1( get_bits_dispo ( etat_gen_ions ) );
    assert( posInactive >= 0);
    //Choisir un bit aleatoire 1
    int posActive = choix_alea_bit1 ( etat_gen_ions );
    assert( posInactive >= 0);
    //Permuter les bits
    etat_gen_ions = flip_bit( etat_gen_ions, posInactive);
    etat_gen_ions = flip_bit( etat_gen_ions, posActive);

    return etat_gen_ions;
}
//******************************|***********************************//

//************************Test des fonctions************************//
void testIsContigue(void)
{
    // Test si aucun contigues
    assert(isContigue(0b1101100110001000110010011010110) == 0);

    // Test si un contigues
    assert(isContigue(0b1101100110001000110010011110110) == 1);
}

void testchoix_alea_bit1(void)
{
    assert(choix_alea_bit1(0) == -1);
    assert(choix_alea_bit1(0xFFFEFFFF) == 16);
}

void testGet_bits_dispo(void)
{
    unsigned int disposition = 9;
    assert( get_bits_dispo( disposition ) == ~disposition);
}
//******************************|***********************************//

//********************Main code to test functions*******************//
// Code just to test functions
#if TEST == 1
int main(void)
{
    srand_sys();
    testIsContigue();
    testchoix_alea_bit1();
    testGet_bits_dispo();
    return 0;
}
#endif

//******************************|***********************************//


//****************************Main Code*****************************//
// Main code to be ran
#if TEST == 0
int main(void)
{
    srand_sys();
    // Etat des generateurs 32bits
    unsigned int etats_gen_ions = initGenerateur();
    
    while (1)
    {
        etats_gen_ions = permuter_bits( etats_gen_ions );
        voir_bits(etats_gen_ions);
    }
    return 0;
}
#endif
//******************************|***********************************//
