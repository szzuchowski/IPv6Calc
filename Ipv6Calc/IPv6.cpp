#include "IPv6.h"
#include <string.h>
#include <iostream>



void IPv6::validateAddress(char *ip) {

    //Przygotowanie tablicy przechowuj¹cej adres ip w pe³nej formie
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            this->charIpSegment[i][j] = '0';
        }
    }
    
    uint8_t x = 0;                                                                  //numer segmentu w tablicy char
    bool inverseFlag = false;                                                       //flaga odpowiedzialna na odwrócenie analizy podanego adresu po znalezioniu ::
    uint8_t doubleColonPos = 0;                                                     //numer pozycji w jakiej zosta³¹ znaleziona ::
    char tempsegment[5] = {'0'};                                                    //tablica tymczasowa dla przechowania pojedyñczego segmentu adresu IP
    int segmenti = 0;                                                               //numer komórki tablicy tymczasowej

    //analiza podanego adresu oraz podzielenie go na segmenty
    for (int i = 0; i < 39; i++) {
        //gdy nie jest w podziale miêdzy segmentami
        if (ip[i] != ':') {
            tempsegment[segmenti] = ip[i];

            segmenti++;
            
        }
        //gdy kursor doszed³ do separatora segmentów
        else {
            //gdy zosta³ podany pe³ny segment XXXX
            if (segmenti == 4) {  
                strcpy_s(this->charIpSegment[x], tempsegment);
                tempsegment[0] = '0';
                tempsegment[1] = '0';
                tempsegment[2] = '0';
                tempsegment[3] = '0';
                x++;
            }
            else { //je¿eli nie to przepisanie z dodaniem nieznacz¹cych 0
                int8_t segi = segmenti-1;
                uint8_t y = 3;
                while (segi >= 0) {
                    this->charIpSegment[x][y] = tempsegment[segi];
                    segi--;
                    y--;
                }
                tempsegment[0] = '0';
                tempsegment[1] = '0';
                tempsegment[2] = '0';
                tempsegment[3] = '0';
                x++;
                
            }
            segmenti = 0;
            if (ip[i + 1] == ':') {                                                 //gdy nastêpna komórka to ":" zapisaæ miejsce znaku, przygotowanie do analizy od koñca adresu IP oraz zakoñczenie tej instancji pêtli for
                inverseFlag = true;
                doubleColonPos = i + 1;
                x = 7;
                break;
            }
        }
    }

    //analiza od koñca adresu IP
    segmenti = 3;
    for (int i = strlen(ip) - 1; i >= doubleColonPos; i--) {                        //pêtla for zaczynaj¹ca od ostaniego znaku w podanym ci¹gu
        if (ip[i] != ':') {                                                         //je¿eli nie znak : to kopiuj to tymczasowej tablicy segmentu 
            tempsegment[segmenti] = ip[i];

            segmenti--;
        }
        else {                                                                      // je¿eli fa³sz skopiuj wartoœæ do tablicy finalnej oraz wyczyœæ.
            segmenti = 3;
            strcpy_s(this->charIpSegment[x], tempsegment);
            tempsegment[0] = '0';
            tempsegment[1] = '0';
            tempsegment[2] = '0';
            tempsegment[3] = '0';
            x--;
            
        }
    }
}

void IPv6::convertDecToBinMask() {
    uint8_t numof16bitFF = this->netMask / 16;                                      //Dziesiêtna iloœæ segmentów maski zawieraj¹cych 0xFFFF
    uint8_t numofbits = this->netMask - (numof16bitFF * 16);                        //iloœæ bitów maski uzupe³niaj¹cych do wartoœci
    for (int i = 0; i < numof16bitFF; i++) {                                        //wype³nienie segmentów maski wartoœci 0xFFFF
        this->binaryNetMask[i] = 0xffff;
    }

    for (uint8_t i = 0; i < 16; i++) {                                              //Pêtla uzupe³niaj¹ca s³owo[WORD] o pozosta³e bity maski oraz przsuwaj¹ca by znajdowa³y siê na swoim miejscu
        if (numofbits > 0) {
            numofbits--;
            this->binaryNetMask[numof16bitFF] = this->binaryNetMask[numof16bitFF] << 1;
            this->binaryNetMask[numof16bitFF] += 0b1;
        } else {
            this->binaryNetMask[numof16bitFF] = this->binaryNetMask[numof16bitFF] << 1;
            this->binaryNetMask[numof16bitFF] += 0b0;
        }

    }
}

uint8_t IPv6::asciiToHex(char c) {
    if (c >= '0' && c <= '9') { //je¿eli podany znak jest z zakresu to od znaku odejmij wartoœæ najni¿szego znaku (interpretacja CHAR jako liczba dziesiêtna Tablica ASCII)
        return c - '0';
    }
    else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    else {
        return -1;
    }
}

void IPv6::getNetwork(uint16_t* address)
{
    for (uint8_t i = 0; i < 8; i++) {
        address[i] = this->binaryNetworkIp[i];
    }
}

void IPv6::getLongAddress(uint16_t address[], char * output)
{
    uint8_t index = 0;
    for (uint8_t i = 0; i < 8; i++) {
        uint16_t tempbin = address[i];

        output[0 + (i * 5)] = this->hexToAscii((tempbin >> 12) & 0x0f);
        output[1 + (i * 5)] = this->hexToAscii((tempbin >> 8) & 0x0f);
        output[2 + (i * 5)] = this->hexToAscii((tempbin >> 4) & 0x0f);
        output[3 + (i * 5)] = this->hexToAscii(tempbin & 0x0f);
        if (i != 7) output[4 + (i * 5)] = ':';

    }
}

void IPv6::getShortAddress(uint16_t address[], char* output)
{
    uint8_t index = 0;
    char addrTemp[8][5] = { 0 };

    // kopia pe³nego adresu do zmiennej tymczasowej
    for (uint8_t i = 0; i < 8; i++) {
        uint16_t tempbin = address[i];

        addrTemp[i][0] = this->hexToAscii((tempbin >> 12) & 0x0f);
        addrTemp[i][1] = this->hexToAscii((tempbin >> 8) & 0x0f);
        addrTemp[i][2] = this->hexToAscii((tempbin >> 4) & 0x0f);
        addrTemp[i][3] = this->hexToAscii(tempbin & 0x0f);

    }

    //wyszukanie miejsca w adresie gdzie znajduj¹ siê puste segmenty 0x0000
    uint8_t start = 255, stop = 255;
    for (uint8_t i = 7; i > 0; i--) {
        if (address[i] == 0 && stop == 255) {
            stop = i;
            start = i;
        }
        if ((address[i-1] == 0) && stop != 255) {
            if (i > 1) {
                if (address[i - 2] == 0) {
                    start = i - 2;
                }
                else {
                    break;
                }
            }
            else { break; }
            
        }
    }

    //wyczyszczenie adresu z nieznacz¹cych zer
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 1; j < 4; j++) {
            if (addrTemp[i][0] == '0') {
                addrTemp[i][0] = addrTemp[i][1];
                addrTemp[i][1] = addrTemp[i][2];
                addrTemp[i][2] = addrTemp[i][3];
                addrTemp[i][3] = addrTemp[i][4];
            }
        }
    }
    index = 0;
    //tworzenie krótkiego adresu na podstawie danych
    for (uint8_t i = 0; i < 8; i++) { 
       
        if (i >= start && i <= stop) {                              //gdy przek³adane s¹ segmenty puste nie wykonuje akcji. Gdy i == stop dodajnie znaku :    
            if (i == stop) {
                output[index] = ':';
                index++;
            }
        }
        else {                                                      //je¿eli fa³sz to kopiuj wartoœæi z tablicy tymczasowej do wyjœciowego ci¹gu znakó
            for (uint8_t j = 0; j < 4; j++) {
                if (addrTemp[i][j] == '\0') {
                    break;
                }
                output[index] = addrTemp[i][j];
                index++;
            }
            if(i!=7) {                                              //dodaj znak : na koñcu segmentu (je¿eli nie ostatni)
                output[index] = ':';
                index++;
            }
            
        }

        
        
    }

}

boost::multiprecision::uint128_t IPv6::getMaxClients()
{
    uint16_t output[8];
    boost::multiprecision::uint128_t value = 0;
    for (uint8_t i = 0; i < 8; i++) {
        output[i] = 0xFFFF;                                 //nadanie segmentowy maksyaln¹ wartoœæ
        output[i] = output[i] & ~this->binaryNetMask[i];    //operacja AND na wartoœci segmentu z odwrotnoœci¹ maski
        value += output[i];                                 //dodanie wartoœci do liczby 128-bitowej
        value = value << 16;                                //przesuniêcie o 16 bitó by w koñcowej fazie znalaz³y siê na odpowiednich miejscach
    }
    return value;
}

void IPv6::convertCharToBinAddress()
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            this->binaryIp[i] = this->binaryIp[i] << 4;
            uint8_t hexval = this->asciiToHex(this->charIpSegment[i][j]);
            if (hexval == -1) break;
            this->binaryIp[i] |= hexval;
        }
    }

}

char IPv6::hexToAscii(uint8_t hex)
{
    if (hex >= 0 && hex <= 9) {
        return hex + '0';
    }
    else if (hex >= 10 && hex <= 15) {
        return (hex - 10) + 'a';
    }
    else {
        return -1;
    }
    
}

void IPv6::setAddress(char *ip) {
    this->validateAddress(ip);
    this->convertCharToBinAddress();
}

void IPv6::setNetMask(uint8_t netmask) {
	this->netMask = netmask;
    this->convertDecToBinMask();
}

void IPv6::makeNetworkAddress()
{
    for (int i = 0; i < 8; i++) {
        this->binaryNetworkIp[i] = this->binaryIp[i] & this->binaryNetMask[i];
    }
}

void IPv6::getMinAddress(uint16_t *address)
{
    for(uint8_t i = 0; i<8; i++) {
        address[i] = this->binaryNetworkIp[i];
    }
}

void IPv6::getMaxAddress(uint16_t *output)
{
    for (uint8_t i = 0; i < 8; i++) {
        output[i] = 0xFFFF;
        output[i] = output[i] & ~this->binaryNetMask[i];
        output[i] += this->binaryNetworkIp[i];
    }
}

void IPv6::getPrevNetwork(uint16_t *output)
{
    boost::multiprecision::uint128_t bigNetworkAddress;

    for (uint8_t i = 0; i < 8; i++) {
        bigNetworkAddress = bigNetworkAddress << 16;
        bigNetworkAddress += this->binaryNetworkIp[i];
    }

    uint8_t reversemask = 128 - this->netMask;
    bigNetworkAddress = bigNetworkAddress >> reversemask;
    bigNetworkAddress -= 1;
    bigNetworkAddress = bigNetworkAddress << reversemask;

    output[0] = (uint16_t)(bigNetworkAddress >> 112) & 0xFFFF;
    output[1] = (uint16_t)(bigNetworkAddress >> 96) & 0xFFFF;
    output[2] = (uint16_t)(bigNetworkAddress >> 80) & 0xFFFF;
    output[3] = (uint16_t)(bigNetworkAddress >> 64) & 0xFFFF;
    output[4] = (uint16_t)(bigNetworkAddress >> 48) & 0xFFFF;
    output[5] = (uint16_t)(bigNetworkAddress >> 32) & 0xFFFF;
    output[6] = (uint16_t)(bigNetworkAddress >> 16) & 0xFFFF;
    output[7] = (uint16_t)(bigNetworkAddress) & 0xFFFF;

}

void IPv6::getNextNetwork(uint16_t* output)
{
    boost::multiprecision::uint128_t bigNetworkAddress;

    for (uint8_t i = 0; i < 8; i++) {
        bigNetworkAddress = bigNetworkAddress << 16;
        bigNetworkAddress += this->binaryNetworkIp[i];
    }

    uint8_t reversemask = 128 - this->netMask;
    bigNetworkAddress = bigNetworkAddress >> reversemask;
    bigNetworkAddress += 1;
    bigNetworkAddress = bigNetworkAddress << reversemask;

    output[0] = (uint16_t)(bigNetworkAddress >> 112) & 0xFFFF;
    output[1] = (uint16_t)(bigNetworkAddress >> 96) & 0xFFFF;
    output[2] = (uint16_t)(bigNetworkAddress >> 80) & 0xFFFF;
    output[3] = (uint16_t)(bigNetworkAddress >> 64) & 0xFFFF;
    output[4] = (uint16_t)(bigNetworkAddress >> 48) & 0xFFFF;
    output[5] = (uint16_t)(bigNetworkAddress >> 32) & 0xFFFF;
    output[6] = (uint16_t)(bigNetworkAddress >> 16) & 0xFFFF;
    output[7] = (uint16_t)(bigNetworkAddress) & 0xFFFF;

}

uint8_t IPv6::getNetmask() {
    return this->netMask;
}


void IPv6::diff3Address(char *address1, char *address2, char *address3) {
    uint8_t maxLen = 50;
    uint8_t minLen = 0;
    uint8_t address1Len = strlen(address1);
    uint8_t address2Len = strlen(address2);
    uint8_t address3Len = strlen(address3);
    uint8_t address1diffStart = 0;
    uint8_t address1diffStop = 0;
    uint8_t address2diffStart = 0;
    uint8_t address2diffStop = 0;
    uint8_t address3diffStart = 0;
    uint8_t address3diffStop = 0;

    if (address1Len < address2Len) {
        maxLen = address2Len;
        minLen = address1Len;
    } 
    else {
        maxLen = address1Len;
        minLen = address2Len;
    }
        

    for (uint8_t i = 0; i < maxLen; i++) {
        if(address1[i] != address2[i]) {
            address1diffStart = i;
            address2diffStart = i;
            break;
        }
    }
    uint8_t tempaddr1 = address1Len;
    uint8_t tempaddr2 = address2Len;
    while(tempaddr1 != 0 || tempaddr2 != 0) {
        if(address1[tempaddr1] != address2[tempaddr2]) {
            address1diffStop = tempaddr1;
            address2diffStop = tempaddr2;
            break;
        }
        tempaddr1--;
        tempaddr2--;
    }


    for (uint8_t i = address1Len; i >= address1diffStart; i--) {
        address1[i + 1] = address1[i];
        if(i == address1diffStart) {
            address1[i] = ' ';
            break;
        }
    }

    for (uint8_t i = address1Len; i >= address1diffStop + 1; i--) {
        address1[i + 1] = address1[i];
        if (i == address1diffStop + 1) {
            address1[i+1] = ' ';
            break;
        }
    }


    for (uint8_t i = 0; i < maxLen; i++) {
        if (address3[i] != address2[i]) {
            address3diffStart = i;
            address2diffStart = i;
            break;
        }
    }


    tempaddr1 = address3Len;
    tempaddr2 = address2Len;
    while (tempaddr1 != 0 || tempaddr2 != 0) {
        if (address3[tempaddr1] != address2[tempaddr2]) {
            address3diffStop = tempaddr1;
            address2diffStop = tempaddr2;
            break;
        }
        tempaddr1--;
        tempaddr2--;
    }


    for (uint8_t i = address3Len; i >= address3diffStart; i--) {
        address3[i + 1] = address3[i];
        if (i == address3diffStart) {
            address3[i] = ' ';
            break;
        }
    }

    for (uint8_t i = address3Len; i >= address3diffStop + 1; i--) {
        address3[i + 1] = address3[i];
        if (i == address3diffStop + 1) {
            address3[i + 1] = ' ';
            break;
        }
    }


    for (uint8_t i = address2Len; i >= address2diffStart; i--) {
        address2[i + 1] = address2[i];
        if (i == address2diffStart) {
            address2[i] = ' ';
            break;
        }
    }

    for (uint8_t i = address2Len; i >= address2diffStop + 1; i--) {
        address2[i + 1] = address2[i];
        if (i == address2diffStop + 1) {
            address2[i + 1] = ' ';
            break;
        }
    }




}