#include "programsetting.h"

programSetting::programSetting(STRUTTURA_PROGRAMMA *struttura)
{
    // per adesso inizializzo tutto a 0 - poi salvero i dati correttamente
    strutturaProgramma=struttura;

    initAll();
}

void programSetting::initAll()
{
    // per adesso inizializzo tutto a 0
    int i;

    strutturaProgramma->programmaMAB=false;
    strutturaProgramma->nomeFileMAB="";


    for (i=0;i<32;i++)
    {
        strutturaProgramma->programmaAttuatori[i]=false;
    }
    strutturaProgramma->nomeFileAttuatori="";

    for (i=0;i<2;i++)
    {
        strutturaProgramma->programmaAssi[i]=false;
    }
    strutturaProgramma->nomeFileAssi="";

    for (i=0;i<4;i++)
    {
        strutturaProgramma->programmaContainer[i]=false;
    }
    strutturaProgramma->nomeFileContainer="";

    for (i=0;i<2;i++)
    {
        strutturaProgramma->programmaCover[i]=false;
    }
    strutturaProgramma->nomeFileCover="";

    strutturaProgramma->programmaAutocap=false;
    strutturaProgramma->nomeFileAutocap="";
    strutturaProgramma->programmaCanLifter=false;
    strutturaProgramma->nomeFileCanLifter="";
    strutturaProgramma->programmaHumidifier=false;
    strutturaProgramma->nomeFileHumidifier="";

}
