#include "IPv6.h"
#include <string.h>
#include <iostream>



void IPv6::validateAddress(char *ip) {

    //Przygotowanie tablicy przechowującej adres ip w pełnej formie
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            this->charIpSegment[i][j] = '0';
        }
    }
    
    uint8_t x = 0;                                                                  //numer segmentu w tablicy char
    bool inverseFlag = false;                                                       //flaga odpowiedzialna na odwrócenie analizy podanego adresu po znalezioniu ::
    uint8_t doubleColonPos = 0;                                                     //numer pozycji w jakiej zostałą znaleziona ::
    char tempsegment[5] = {'0'};                                                    //tablica tymczasowa dla przechowania pojedyńczego segmentu adresu IP
    int segmenti = 0;                                                               //numer komórki tablicy tymczasowej

    //analiza podanego adresu oraz podzielenie go na segmenty
    for (int i = 0; i < 39; i++) {
        //gdy nie jest w podziale między segmentami
        if (ip[i] != ':') {
            tempsegment[segmenti] = ip[i];

            segmenti++;
            
        }
        //gdy kursor doszedł do separatora segmentów
        else {
            //gdy został podany pełny segment XXXX
            if (segmenti == 4) {  
                strcpy_s(this->charIpSegment[x], tempsegment);
                tempsegment[0] = '0';
                tempsegment[1] = '0';
                tempsegment[2] = '0';
                tempsegment[3] = '0';
                x++;
            }
            else { //jeżeli nie to przepisanie z dodaniem nieznaczących 0
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
            if (ip[i + 1] == ':') {                                                 //gdy następna komórka to ":" zapisać miejsce znaku, przygotowanie do analizy od końca adresu IP oraz zakończenie tej instancji pętli for
                inverseFlag = true;
                doubleColonPos = i + 1;
                x = 7;
                break;
            }
        }
    }

    //analiza od końca adresu IP
    segmenti = 3;
    for (int i = strlen(ip) - 1; i >= doubleColonPos; i--) {                        //pętla for zaczynająca od ostaniego znaku w podanym ciągu
        if (ip[i] != ':') {                                                         //jeżeli nie znak : to kopiuj to tymczasowej tablicy segmentu 
            tempsegment[segmenti] = ip[i];

            segmenti--;
        }
        else {                                                                      // jeżeli fałsz skopiuj wartość do tablicy finalnej oraz wyczyść.
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
    uint8_t numof16bitFF = this->netMask / 16;                                      //Dziesiętna ilość segmentów maski zawierających 0xFFFF
    uint8_t numofbits = this->netMask - (numof16bitFF * 16);                        //ilość bitów maski uzupełniających do wartości
    for (int i = 0; i < numof16bitFF; i++) {                                        //wypełnienie segmentów maski wartości 0xFFFF
        this->binaryNetMask[i] = 0xffff;
    }

    for (uint8_t i = 0; i < 16; i++) {                                              //Pętla uzupełniająca słowo[WORD] o pozostałe bity maski oraz przsuwająca by znajdowały się na swoim miejscu
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
    if (c >= '0' && c <= '9') { //jeżeli podany znak jest z zakresu to od znaku odejmij wartość najniższego znaku (interpretacja CHAR jako liczba dziesiętna Tablica ASCII)
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

    // kopia pełnego adresu do zmiennej tymczasowej
    for (uint8_t i = 0; i < 8; i++) {
        uint16_t tempbin = address[i];

        addrTemp[i][0] = this->hexToAscii((tempbin >> 12) & 0x0f);
        addrTemp[i][1] = this->hexToAscii((tempbin >> 8) & 0x0f);
        addrTemp[i][2] = this->hexToAscii((tempbin >> 4) & 0x0f);
        addrTemp[i][3] = this->hexToAscii(tempbin & 0x0f);

    }

    //wyszukanie miejsca w adresie gdzie znajdują się puste segmenty 0x0000
    uint8_t start = 255, stop = 255;
    bool nothingToDo = false;
    for (uint8_t i = 7; i > 0; i--) {
        if (address[i] == 0 && stop == 255) {
            stop = i;
            start = i;
        }
        if (stop != 255) {
            if (nothingToDo) continue;
            if (address[i] == 0) {
                    start = i;
            }
            else { 
                nothingToDo = true;
                break; 
            }
            
        }
    }

    //wyczyszczenie adresu z nieznaczących zer
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
       
        if (i >= start && i <= stop) {                              //gdy przekładane są segmenty puste nie wykonuje akcji. Gdy i == stop dodajnie znaku :    
            if (i == stop) {
                output[index] = ':';
                index++;
            }
        }
        else {                                                      //jeżeli fałsz to kopiuj wartośći z tablicy tymczasowej do wyjściowego ciągu znakó
            for (uint8_t j = 0; j < 4; j++) {
                if (addrTemp[i][j] == '\0') {
                    break;
                }
                output[index] = addrTemp[i][j];
                index++;
            }
            if(i!=7) {                                              //dodaj znak : na końcu segmentu (jeżeli nie ostatni)
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
        output[i] = 0xFFFF;                                 //nadanie segmentowy maksyalną wartość
        output[i] = output[i] & ~this->binaryNetMask[i];    //operacja AND na wartości segmentu z odwrotnością maski
        value += output[i];                                 //dodanie wartości do liczby 128-bitowej
        if(i!=7) {
            value = value << 16;                                //przesunięcie o 16 bitó by w końcowej fazie znalazły się na odpowiednich miejscach
        }
        
    }
    return value + 1;
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
            address1diffStart = i-1;
            address2diffStart = i-1;
            break;
        }
    }
    uint8_t tempaddr1 = address1Len;
    uint8_t tempaddr2 = address2Len;
    while(tempaddr1 != 0 || tempaddr2 != 0) {
        if(address1[tempaddr1] != address2[tempaddr2]) {
            address1diffStop = tempaddr1+1;
            address2diffStop = tempaddr2+1;
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

    for (uint8_t i = address1Len+1; i >= address1diffStop ; i--) {
        address1[i + 1] = address1[i];
        if (i == address1diffStop + 1) {
            address1[i] = ' ';
            break;
        }
    }


    for (uint8_t i = 0; i < maxLen; i++) {
        if (address3[i] != address2[i]) {
            address3diffStart = i-1;
            if(address2diffStart > i-1) {
                address2diffStart = i - 1;
                
            } else {
                if (address3diffStart > address2diffStart) {
                    address3diffStart = address2diffStart;
                }
            }
            
            break;
        }
    }


    tempaddr1 = address3Len;
    tempaddr2 = address2Len;
    while (tempaddr1 != 0 || tempaddr2 != 0) {
        if (address3[tempaddr1] != address2[tempaddr2]) {
            address3diffStop = tempaddr1+1;
            address2diffStop = tempaddr2+1;
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

    for (uint8_t i = address3Len+1; i >= address3diffStop + 1; i--) {
        address3[i + 1] = address3[i];
        if (i == address3diffStop + 1) {
            address3[i] = ' ';
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

    for (uint8_t i = address2Len+1; i >= address2diffStop + 1; i--) {
        address2[i + 1] = address2[i];
        if (i == address2diffStop + 1) {
            address2[i] = ' ';
            break;
        }
    }




}
