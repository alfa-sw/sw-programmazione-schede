import usb.core
import usb.util
import time
import string
import fileinput
import sys
import threading
import math
import os
from time import sleep
import json
import os.path
    
HEX_FILE_EXTENDED_LINEAR_ADDRESS=0x04
HEX_FILE_EOF=0x01
HEX_FILE_DATA=0x00
DEVICE_FAMILY_PIC18=1
DEVICE_FAMILY_PIC24=2
DEVICE_FAMILY_PIC32=3
MEMORY_REGION_CONFIG=0x03
PIC24_RESET_REMAP_OFFSET=0x1400


class MEMORYREGION():
    def __init__(self,Type,Address,Size):
        self.Type=Type
        self.Address=Address
        self.Size=Size
    
    def setType(self,tipo):
        self.Type=tipo
    def getType(self):
        return self.Type
    def setAddress(self,indirizzo):
        self.Address=indirizzo
    def getAddress(self):
        return self.Address
    def setSize(self,dim):
        self.Size=dim
    def getSize(self):
        return self.Size
    
    
class programmaFirmware():

    pwd=[0x82,0x14,0x2A,0x5D,0x6F,0x9A,0x25,0x01]
    rawData=bytearray(64)
    pData0=bytearray()
    pData1=bytearray()
    pData2=bytearray()
    pData3=bytearray()
    pData4=bytearray()
    pData5=bytearray()
    
    pData=[pData0,pData1,pData2,pData3,pData4,pData5] 
    recordLength=0
    addressField=0
    recordType=0
    checksum=0
    dataPayload=""
    extendedAddress=0
    memoryRegion=[]
    memoryRegionsDetected=0
    bytesPerPacket=0
    bytesPerAddress=0
    
    def spedisciUSB(self,dispositivoBoot,dati,timeout,numCaratteri):
        dispositivoBoot.set_configuration()
        cfg = dispositivoBoot.get_active_configuration()
        intf = cfg[(0,0)]
        ep = usb.util.find_descriptor(
            intf,
            # match the first OUT endpoint
            custom_match = \
            lambda e: \
            usb.util.endpoint_direction(e.bEndpointAddress) == \
            usb.util.ENDPOINT_OUT)

        #adesso spedisco il comando di reset e mi aspetto la risposta
        try:
            ret=dispositivoBoot.write(ep,dati,timeout)
        except Exception:
            return
    
        if ret!=numCaratteri+1:
            return
    
        return ret
    
    def connettiDispositivo(self):
        dev = usb.core.find(idVendor=0x04d8, idProduct=0xe89b)
    
        # was it found?
        if dev is None:
            return
    
        return dev
    
    def spedisciQuery(self,dispositivoBoot,dispositivoID,timeout):
        # in questa funzione spediamo il comando di query e aspettiamo la risposta
        
        self.rawData[0]=2 #comando QUERY
        for i in range(0,8):
            self.rawData[i+1]=self.pwd[i]
    
        self.rawData[9]=dispositivoID #ID della MAB    
        for i in range(10,64):
            self.rawData[i]=0
    
        #adesso devo spedire il comando di query ed attendere che il bootloader mi risponda
        ret=self.spedisciUSB(dispositivoBoot, self.rawData, 1000, len(self.rawData))
        if ret is None:
            return
    
        #adesso aspetto che il bootloader mi risponda alla query
        if ret==65:
            #ha trasmesso corretto - adesso aspetto la risposta
            dispositivoBoot.set_configuration()
            cfg = dispositivoBoot.get_active_configuration()
            intf = cfg[(0,0)]
            ep = usb.util.find_descriptor(
                intf,
                # match the first OUT endpoint
                custom_match = \
                lambda e: \
                usb.util.endpoint_direction(e.bEndpointAddress) == \
                usb.util.ENDPOINT_IN)
        
            try:
                rawData=dispositivoBoot.read(ep,64,timeout)
            except:
                return
        
            if len(rawData)!=64:
                return
        else:
            return
    
        return rawData

    def spedisciRead(self,dispositivoBoot):
        dispositivoBoot.set_configuration()
        cfg = dispositivoBoot.get_active_configuration()
        intf = cfg[(0,0)]
        ep = usb.util.find_descriptor(
            intf,
            # match the first OUT endpoint
            custom_match = \
            lambda e: \
            usb.util.endpoint_direction(e.bEndpointAddress) == \
            usb.util.ENDPOINT_OUT)

        #adesso spedisco il comando di reset e mi aspetto la risposta
        dispositivoBoot.write(ep,self.rawData,100)
    
    def spedisciErase(self,dispositivoBoot,IDdispositivo):
        #devo spedire il comando di erase - costruisco la struttura dati
        #prima devo spedire il comando di query poi devo spedire il comando di erase
    
        rawData=self.spedisciQuery(dispositivoBoot, IDdispositivo,1000)
    
        if rawData is None:
            return
    
        #adesso devo spedire il comando di erase
        self.rawData[0]=0x04   # comando di erase
    
        for i in range(1,64):
            self.rawData[i]=0
    
        ret=self.spedisciUSB(dispositivoBoot, self.rawData, 1000, len(rawData))
        if ret is None:
            return
    
        #adesso devo aspettare che finisca di fare l'erase
        #per sapere se ha finito devo spedire un comando di query
        rawData=self.spedisciQuery(dispositivoBoot, IDdispositivo,60000)
        if rawData is None:
            return
        #se mi ha risposto ha finito di fare l'erase
        return 1
    
    def StringToHex(self,stringaToConvert):
        #converto una stringa da esadecimale a numero
        returnAddress=0
        placeMultiplier=1
    
        for i in range(0,len(stringaToConvert)):
            c = stringaToConvert[len(stringaToConvert) - 1 - i]
            car=0
            if (c >= 'A') and (c <= 'F'):
                car = 10 + (int(c,16) - 10)
            elif (c >= 'a') and (c <= 'f'):
                car = 10 + (int(c,16) - 10)
            else:
                car = int(c,16)

            returnAddress = returnAddress + (car * placeMultiplier)
            placeMultiplier = placeMultiplier * 16

        return returnAddress
    def caricaFileHex(self,dispositivoBoot,idDispositivo,nomeFile):
        #questa funzione carica tutti i dati relativi ad un file hex
        self.memoryRegion.clear()
        rawData=self.spedisciQuery(dispositivoBoot, idDispositivo,1000)
        if rawData is None:
            return
    
        self.bytesPerPacket=rawData[1]
        self.deviceFamily=rawData[2]
    
        contaByte=3
        tipo=0
        indirizzo=0
        dimensione=0
        appoggio=0;
        appoggio1=0
        finito=False
        j=0
        self.memoryRegionsDetected=0
        tmp=0
    
        while finito==False:
            tipo=rawData[contaByte]
            if tipo==255:
                #terminata la cosa devo uscire dal ciclo for
                finito=True
                break
            
            #calcolo l'indirizzo della regione
            contaByte+=1
            appoggio=rawData[contaByte]
            contaByte+=1
            tmp=rawData[contaByte]
            tmp=tmp<<8
            appoggio+=tmp
            contaByte+=1
            appoggio1=rawData[contaByte]
            contaByte+=1
            tmp=rawData[contaByte]
            tmp=tmp<<8
            appoggio1+=tmp
            contaByte+=1
            appoggio1=appoggio1<<16
            indirizzo=appoggio+appoggio1
            
            #calcolo la dimensione della regione di memoria
            appoggio=rawData[contaByte]
            contaByte+=1
            tmp=rawData[contaByte]
            tmp=tmp<<8
            appoggio+=tmp
            contaByte+=1
            appoggio1=rawData[contaByte]
            contaByte+=1
            tmp=rawData[contaByte]
            tmp=tmp<<8
            appoggio1+=tmp
            contaByte+=1
            appoggio1=appoggio1<<16
            dimensione=appoggio+appoggio1
            self.memoryRegion.append(MEMORYREGION(tipo,indirizzo,dimensione))
            self.memoryRegionsDetected+=1
            j+=1
            if j>=6:
                finito=True
    
        #definisco il numero di bytes per address sulla base del tipo di micro
        self.bytesPerAddress=0
        if self.deviceFamily==DEVICE_FAMILY_PIC18:
            self.bytesPerAddress=1
        elif self.deviceFamily==DEVICE_FAMILY_PIC24:
            self.bytesPerAddress=2
        elif self.deviceFamily==DEVICE_FAMILY_PIC32:
            self.bytesPerAddress=3
    
        #for i in range(0,(self.memoryRegion[0].getSize()+1)*self.bytesPerAddress):
        self.pData[0]=bytearray((self.memoryRegion[0].getSize()+1)*self.bytesPerAddress)
    
        if self.memoryRegionsDetected>1:
            #for i in range(0,(self.memoryRegion[1].getSize()+1)*self.bytesPerAddress):
            self.pData[1]=bytearray((self.memoryRegion[1].getSize()+1)*self.bytesPerAddress)
            if self.memoryRegionsDetected>2:
                #for i in range(0,(self.memoryRegion[2].getSize()+1)*self.bytesPerAddress):
                self.pData[2]=bytearray((self.memoryRegion[2].getSize()+1)*self.bytesPerAddress)
                if self.memoryRegionsDetected>3:
                    #for i in range(0,(self.memoryRegion[3].getSize()+1)*self.bytesPerAddress):
                    self.pData[3]=bytearray((self.memoryRegion[3].getSize()+1)*self.bytesPerAddress)
                    if self.memoryRegionsDetected>4:
                        #for i in range(0,(self.memoryRegion[4].getSize()+1)*self.bytesPerAddress):
                        self.pData[4]=bytearray((self.memoryRegion[4].getSize()+1)*self.bytesPerAddress)
                        if self.memoryRegionsDetected>5:
                            #for i in range(0,(self.memoryRegion[5].getSize()+1)*self.bytesPerAddress):
                            self.pData[4]=bytearray((self.memoryRegion[4].getSize()+1)*self.bytesPerAddress)
    
        #adesso ho le dimensioni delle regioni di memoria
        fileHex=open(nomeFile,'r')
        hexFileEOF=False
    
        rigaFile=fileHex.readline()
        while len(rigaFile)>0 and hexFileEOF==False:
            #devo scorrere tutto il file 
            rigaFile=rigaFile.strip()
            if len(rigaFile)>0:
                if rigaFile[0]!=':':
                    #non comincia nel modo giusto
                    fileHex.close()
                    return
            
                rigaFile=rigaFile.strip(':')
                recordLength=self.StringToHex(rigaFile[0:2])
                temp=rigaFile[2:6]
                addressField=self.StringToHex(rigaFile[2:6])
                recordType=self.StringToHex(rigaFile[6:8])
                dataPayload=rigaFile[8:8+recordLength*2]
                checksum=self.StringToHex(rigaFile[8+recordLength*2:10+recordLength*2])
            
                checksumCalculated=0
                for j in range(0,recordLength+4):
                    checksumCalculated+=self.StringToHex(rigaFile[j*2:(j*2)+2])
            
                checksumCalculated=~checksumCalculated
                checksumCalculated+=1
                checksumCalculated=checksumCalculated & 0x000000FF
            
                if (checksumCalculated & 0x000000FF) !=checksum:
                    fileHex.close()
                    return
            
                #andiamo avanti
                if recordType==HEX_FILE_EXTENDED_LINEAR_ADDRESS:
                    self.extendedAddress=self.StringToHex(dataPayload)
                elif recordType == HEX_FILE_EOF:
                    hexFileEOF=True
                elif recordType==HEX_FILE_DATA:
                    savedData=False
                    foundMemoryRegion=False
                    totalAddress=(self.extendedAddress << 16)+addressField
                    contaRegioni=0
                
                    for j in range(0,self.memoryRegionsDetected):
                        if (totalAddress>=self.memoryRegion[j].getAddress()*self.bytesPerAddress) and (totalAddress<(self.memoryRegion[j].getAddress()+self.memoryRegion[j].getSize())*self.bytesPerAddress):
                            for i in range(0,recordLength):
                                dato=self.StringToHex(dataPayload[i*2:i*2+2])
                                p=totalAddress-(self.memoryRegion[j].getAddress()*self.bytesPerAddress) + i
                                limit=(self.memoryRegion[j].getSize()+1)*self.bytesPerAddress
                                if (p>=limit):
                                    break
                            
                                if self.bytesPerAddress==2:
                                    if (totalAddress+i)<0x06:
                                        if (totalAddress+i)==0:
                                            self.pData[j][0]=0
                                        elif (totalAddress+i)==1 or (totalAddress+i)==2:
                                            self.pData[j][i]=4
                                        elif (totalAddress+i)==3 or (totalAddress+i)==4 or (totalAddress+i)==5:
                                            self.pData[j][i]=0
                                        else:
                                            self.pData[j][0]=dato
                                    
                                        for k in range(0,self.memoryRegionsDetected):
                                            if self.memoryRegion[k].getAddress()==0x1400:
                                                self.pData[k][totalAddress+i]=dato
                                    elif (totalAddress+i)==(0x1400*2):
                                        l=0
                                    elif (totalAddress+i)==(0x1400*2)+1:
                                        l=1
                                    elif (totalAddress+i)==(0x1400*2)+2:
                                        l=2
                                    elif (totalAddress+i)==(0x1400*2)+3:
                                        l=3
                                    elif (totalAddress+i)==(0x1400*2)+4:
                                        l=4
                                    elif (totalAddress+i)==(0x1400*2)+5:
                                        l=5
                                    else:
                                        self.pData[j][totalAddress-(self.memoryRegion[j].getAddress()*self.bytesPerAddress) + i]=dato
                            
                                else:
                                    self.pData[j][totalAddress-(self.memoryRegion[j].getAddress()*self.bytesPerAddress) + i]=dato
                            
                            break;
                        
                else:
                    break;
            
            rigaFile=fileHex.readline()                        
    
        fileHex.close()
        
        return 1
    
    def verificaProgram(self,dispositivoBoot,idDispositivo):
        #questa funzione da per scontato che in memory region siano già stati caricati tutti i dati - questo è vero
        #nel caso della programmazione, mentre nel caso in cui venga chiesta una verifica si dovrà prima chiamare
        #la funzione caricaFileHex
        packetsWritten=0
        packetsRead=0
        BytesWritten=0
        BytesReceived=0
        rawDataAnswer=bytearray(64)
        p=0
        pSave=0
        
        for currentMemoryRegion in range(0,self.memoryRegionsDetected):
            if self.memoryRegion[currentMemoryRegion].getType()==3:
                #la regione è la regione dei bit di configurazione che non vengono programmati
                continue
            
            addressToRequest=self.memoryRegion[currentMemoryRegion].getAddress()
            dimensione=self.memoryRegion[currentMemoryRegion].getSize()
            contaByte=0
            contaSave=0
            
            while addressToRequest<(self.memoryRegion[currentMemoryRegion].getAddress()+self.memoryRegion[currentMemoryRegion].getSize()):
                self.rawData[0]=0x07   #comando GET_DATA
                #carico l'indirizzo
                appoggio=addressToRequest & 0x00FFFF
                indice=1
                self.rawData[indice]=(appoggio & 0x00FF)
                indice+=1
                self.rawData[indice]=(appoggio & 0xFF00)>>8
                indice+=1
                appoggio=addressToRequest & 0xFFFF0000
                appoggio=appoggio>>16
                self.rawData[indice]=(appoggio & 0x00FF)
                indice+=1
                self.rawData[indice]=(appoggio & 0xFF00)>>8
                indice+=1
                #carico il numero di byte per packet
                self.rawData[indice]=self.bytesPerPacket
                indice+=1
                
                if (addressToRequest+int(self.bytesPerPacket/self.bytesPerAddress))>(self.memoryRegion[currentMemoryRegion].getAddress()+self.memoryRegion[currentMemoryRegion].getSize()):
                    self.rawData[indice-1]=self.bytesPerPacket-((addressToRequest+int(self.bytesPerPacket/self.bytesPerAddress)-(self.memoryRegion[currentMemoryRegion].getAddress()+self.memoryRegion[currentMemoryRegion].getSize())))*self.bytesPerAddress
                
                while (self.rawData[indice-1] % self.bytesPerAddress)!=0:
                    self.rawData[indice-1]+=1
                
                rit=self.spedisciUSB(dispositivoBoot, self.rawData, 3000, 64)
                if rit is None:
                    return
                packetsWritten+=1
                addressToRequest+=int(self.bytesPerPacket/self.bytesPerAddress)
                
                dispositivoBoot.set_configuration()
                cfg = dispositivoBoot.get_active_configuration()
                intf = cfg[(0,0)]
                ep = usb.util.find_descriptor(
                    intf,
                    # match the first OUT endpoint
                    custom_match = \
                    lambda e: \
                    usb.util.endpoint_direction(e.bEndpointAddress) == \
                    usb.util.ENDPOINT_IN)
        
                try:
                    rawDataAnswer=dispositivoBoot.read(ep,64,10000)
                except:
                    return
                
                if len(rawDataAnswer)!=64:
                    return
                
                #adesso devo andare a leggere il contenuto
                responseAddress=0
                indiceAnswer=1
                appoggio=rawDataAnswer[indiceAnswer]
                indiceAnswer+=1
                tmp=rawDataAnswer[indiceAnswer]
                tmp=tmp<<8
                appoggio+=tmp
                indiceAnswer+=1
                appoggio1=rawDataAnswer[indiceAnswer]
                indiceAnswer+=1
                tmp=rawDataAnswer[indiceAnswer]
                tmp=tmp<<8
                appoggio1+=tmp
                indiceAnswer+=1
                appoggio1=appoggio1<<16
                responseAddress=appoggio+appoggio1
                
                bytePerPacketAnswer=rawDataAnswer[indiceAnswer]
                indiceAnswer+=1
                
                if responseAddress==0 or bytePerPacketAnswer==0 or rawDataAnswer[0]==0:
                    #problema di trasmissione
                    return
                
                packetsRead+=1
                pSave=p+(responseAddress-self.memoryRegion[currentMemoryRegion].getAddress())*self.bytesPerAddress
                for i in range(0,bytePerPacketAnswer):
                    if pSave<len(self.pData[currentMemoryRegion]):
                        tempData=self.pData[currentMemoryRegion][pSave]
                    else:
                        tempData=0
                    
                    pSave+=1
                    tempData2=rawDataAnswer[indiceAnswer+bytePerPacketAnswer+2-bytePerPacketAnswer+i]
                    if tempData!=tempData2:
                        if (self.bytesPerAddress==2) and ((i+1) % 4)==0:
                            #questo è il quarto byte per cui possiamo evitare di controllarlo
                            tempData=0
                        elif self.bytesPerAddress==2 and (responseAddress==PIC24_RESET_REMAP_OFFSET):
                            for k in range(0,self.memoryRegionsDetected):
                                if self.memoryRegion[k].getAddress()==PIC24_RESET_REMAP_OFFSET:
                                    tempPtr=k
                            
                            if i==0:
                                if tempData2!=self.pData[tempPtr][0]:
                                    if tempData2!=self.pData[tempPtr][1]:
                                        if tempData2!=self.pData[tempPtr][2]:
                                            if tempData2!=self.pData[tempPtr][3]:
                                                if tempData2!=self.pData[tempPtr][4]:
                                                    if tempData2!=self.pData[tempPtr][5]:
                                                        return
                            elif i==1:
                                if tempData2!=self.pData[tempPtr][1]:
                                    if tempData2!=self.pData[tempPtr][2]:
                                        if tempData2!=self.pData[tempPtr][3]:
                                            if tempData2!=self.pData[tempPtr][4]:
                                                if tempData2!=self.pData[tempPtr][5]:
                                                    return
                            elif i==2:
                                if tempData2!=self.pData[tempPtr][2]:
                                    if tempData2!=self.pData[tempPtr][3]:
                                        if tempData2!=self.pData[tempPtr][4]:
                                            if tempData2!=self.pData[tempPtr][5]:
                                                return
                            elif i==3:
                                if tempData2!=self.pData[tempPtr][3]:
                                    if tempData2!=self.pData[tempPtr][4]:
                                        if tempData2!=self.pData[tempPtr][5]:
                                            return
                            elif i==4:
                                if tempData2!=self.pData[tempPtr][4]:
                                    if tempData2!=self.pData[tempPtr][5]:
                                        return
                            elif i==5:
                                if tempData2!=self.pData[tempPtr][5]:
                                    return
                            else:
                                return
                        elif self.bytesPerAddress==2 and responseAddress==0:
                            if i==0:
                                if tempData2!=0:
                                    if tempData2!=4:
                                        return
                            elif i==1:
                                if tempData2!=4 and tempData2!=0:
                                    return
                            elif i==2:
                                if tempData2!=4 and tempData2!=0:
                                    return
                            elif i==3:
                                if tempData2!=0:
                                    return
                            elif i==4:
                                if tempData2!=0:
                                    return
                            elif i==5:
                                if tempData2!=0:
                                    return
                            else:
                                return
                        else:
                            return
                                
        return 1    
    
    def gestisciProgrammazione(self,dispositivoBoot,nomeFile,idDispositivo,ricaricaMem):
        # in questa funzione apriamo il file passato come parametro e scriviamo su dispositivo boot la programmazione
        #la prima cosa da fare è verificare che il file ci sia
        if ricaricaMem:
            self.memoryRegion.clear()
            rawData=self.spedisciQuery(dispositivoBoot, idDispositivo,1000)
            if rawData is None:
                return
    
            self.bytesPerPacket=rawData[1]
            self.deviceFamily=rawData[2]
    
            contaByte=3
            tipo=0
            indirizzo=0
            dimensione=0
            appoggio=0;
            appoggio1=0
            finito=False
            j=0
            self.memoryRegionsDetected=0
            tmp=0
    
            while finito==False:
                tipo=rawData[contaByte]
                if tipo==255:
                    #terminata la cosa devo uscire dal ciclo for
                    finito=True
                    break
            
                #calcolo l'indirizzo della regione
                contaByte+=1
                appoggio=rawData[contaByte]
                contaByte+=1
                tmp=rawData[contaByte]
                tmp=tmp<<8
                appoggio+=tmp
                contaByte+=1
                appoggio1=rawData[contaByte]
                contaByte+=1
                tmp=rawData[contaByte]
                tmp=tmp<<8
                appoggio1+=tmp
                contaByte+=1
                appoggio1=appoggio1<<16
                indirizzo=appoggio+appoggio1
            
                #calcolo la dimensione della regione di memoria
                appoggio=rawData[contaByte]
                contaByte+=1
                tmp=rawData[contaByte]
                tmp=tmp<<8
                appoggio+=tmp
                contaByte+=1
                appoggio1=rawData[contaByte]
                contaByte+=1
                tmp=rawData[contaByte]
                tmp=tmp<<8
                appoggio1+=tmp
                contaByte+=1
                appoggio1=appoggio1<<16
                dimensione=appoggio+appoggio1
                self.memoryRegion.append(MEMORYREGION(tipo,indirizzo,dimensione))
                self.memoryRegionsDetected+=1
                j+=1
                if j>=6:
                    finito=True
    
            #definisco il numero di bytes per address sulla base del tipo di micro
            self.bytesPerAddress=0
            if self.deviceFamily==DEVICE_FAMILY_PIC18:
                self.bytesPerAddress=1
            elif self.deviceFamily==DEVICE_FAMILY_PIC24:
                self.bytesPerAddress=2
            elif self.deviceFamily==DEVICE_FAMILY_PIC32:
                self.bytesPerAddress=3
    
            #for i in range(0,(self.memoryRegion[0].getSize()+1)*self.bytesPerAddress):
            self.pData[0]=bytearray((self.memoryRegion[0].getSize()+1)*self.bytesPerAddress)
    
            if self.memoryRegionsDetected>1:
                #for i in range(0,(self.memoryRegion[1].getSize()+1)*self.bytesPerAddress):
                self.pData[1]=bytearray((self.memoryRegion[1].getSize()+1)*self.bytesPerAddress)
                if self.memoryRegionsDetected>2:
                    #for i in range(0,(self.memoryRegion[2].getSize()+1)*self.bytesPerAddress):
                    self.pData[2]=bytearray((self.memoryRegion[2].getSize()+1)*self.bytesPerAddress)
                    if self.memoryRegionsDetected>3:
                        #for i in range(0,(self.memoryRegion[3].getSize()+1)*self.bytesPerAddress):
                        self.pData[3]=bytearray((self.memoryRegion[3].getSize()+1)*self.bytesPerAddress)
                        if self.memoryRegionsDetected>4:
                            #for i in range(0,(self.memoryRegion[4].getSize()+1)*self.bytesPerAddress):
                            self.pData[4]=bytearray((self.memoryRegion[4].getSize()+1)*self.bytesPerAddress)
                            if self.memoryRegionsDetected>5:
                                #for i in range(0,(self.memoryRegion[5].getSize()+1)*self.bytesPerAddress):
                                self.pData[4]=bytearray((self.memoryRegion[4].getSize()+1)*self.bytesPerAddress)
        
        else:
            rawData=self.spedisciQuery(dispositivoBoot, idDispositivo,1000)
            if rawData is None:
                return
        
        if ricaricaMem:
            #adesso ho le dimensioni delle regioni di memoria
            fileHex=open(nomeFile,'r')
            hexFileEOF=False
    
            rigaFile=fileHex.readline()
            while len(rigaFile)>0 and hexFileEOF==False:
                #devo scorrere tutto il file 
                rigaFile=rigaFile.strip()
                if len(rigaFile)>0:
                    if rigaFile[0]!=':':
                        #non comincia nel modo giusto
                        fileHex.close()
                        return
            
                    rigaFile=rigaFile.strip(':')
                    recordLength=self.StringToHex(rigaFile[0:2])
                    temp=rigaFile[2:6]
                    addressField=self.StringToHex(rigaFile[2:6])
                    recordType=self.StringToHex(rigaFile[6:8])
                    dataPayload=rigaFile[8:8+recordLength*2]
                    checksum=self.StringToHex(rigaFile[8+recordLength*2:10+recordLength*2])
            
                    checksumCalculated=0
                    for j in range(0,recordLength+4):
                        checksumCalculated+=self.StringToHex(rigaFile[j*2:(j*2)+2])
            
                    checksumCalculated=~checksumCalculated
                    checksumCalculated+=1
                    checksumCalculated=checksumCalculated & 0x000000FF
            
                    if (checksumCalculated & 0x000000FF) !=checksum:
                        fileHex.close()
                        return
            
                    #andiamo avanti
                    if recordType==HEX_FILE_EXTENDED_LINEAR_ADDRESS:
                        self.extendedAddress=self.StringToHex(dataPayload)
                    elif recordType == HEX_FILE_EOF:
                        hexFileEOF=True
                    elif recordType==HEX_FILE_DATA:
                        savedData=False
                        foundMemoryRegion=False
                        totalAddress=(self.extendedAddress << 16)+addressField
                        contaRegioni=0
                
                        for j in range(0,self.memoryRegionsDetected):
                            if (totalAddress>=self.memoryRegion[j].getAddress()*self.bytesPerAddress) and (totalAddress<(self.memoryRegion[j].getAddress()+self.memoryRegion[j].getSize())*self.bytesPerAddress):
                                for i in range(0,recordLength):
                                    dato=self.StringToHex(dataPayload[i*2:i*2+2])
                                    p=totalAddress-(self.memoryRegion[j].getAddress()*self.bytesPerAddress) + i
                                    limit=(self.memoryRegion[j].getSize()+1)*self.bytesPerAddress
                                    if (p>=limit):
                                        break
                            
                                    if self.bytesPerAddress==2:
                                        if (totalAddress+i)<0x06:
                                            if (totalAddress+i)==0:
                                                self.pData[j][0]=0
                                            elif (totalAddress+i)==1 or (totalAddress+i)==2:
                                                self.pData[j][i]=4
                                            elif (totalAddress+i)==3 or (totalAddress+i)==4 or (totalAddress+i)==5:
                                                self.pData[j][i]=0
                                            else:
                                                self.pData[j][0]=dato
                                    
                                            for k in range(0,self.memoryRegionsDetected):
                                                if self.memoryRegion[k].getAddress()==0x1400:
                                                    self.pData[k][totalAddress+i]=dato
                                        elif (totalAddress+i)==(0x1400*2):
                                            l=0
                                        elif (totalAddress+i)==(0x1400*2)+1:
                                            l=1
                                        elif (totalAddress+i)==(0x1400*2)+2:
                                            l=2
                                        elif (totalAddress+i)==(0x1400*2)+3:
                                            l=3
                                        elif (totalAddress+i)==(0x1400*2)+4:
                                            l=4
                                        elif (totalAddress+i)==(0x1400*2)+5:
                                            l=5
                                        else:
                                            self.pData[j][totalAddress-(self.memoryRegion[j].getAddress()*self.bytesPerAddress) + i]=dato
                            
                                    else:
                                        self.pData[j][totalAddress-(self.memoryRegion[j].getAddress()*self.bytesPerAddress) + i]=dato
                            
                                break;
                        
                    else:
                        break;
            
                rigaFile=fileHex.readline()                        
    
            fileHex.close()
                                    
        #ho caricato il file hex
        #adesso devo programmare lo slave
    
        #spediamo un altro comando di query
        rawData=self.spedisciQuery(dispositivoBoot, idDispositivo,30000)
        if rawData is None:
            return
    
        configsProgrammed=True
        everythingElseProgrammed=False
        skipBlock=False
        blockSkipped=False
        currentByteInAddress=0
    
        while (configsProgrammed==False) or (everythingElseProgrammed==False):
            #ciclo principale di programmazione
            for currentMemoryRegion in range(0,self.memoryRegionsDetected):
                if configsProgrammed==False:
                    if self.memoryRegion[currentMemoryRegion].getType()!=MEMORY_REGION_CONFIG:
                        continue
                else:
                    if self.memoryRegion[currentMemoryRegion].getType()==MEMORY_REGION_CONFIG:
                        continue
            
                address=self.memoryRegion[currentMemoryRegion].getAddress()
                dimensione=self.memoryRegion[currentMemoryRegion].getSize()
                contaDati=0
                skipBlock=True
                blockSkipped=False
                currentByteInAddress=1
            
                while address<(self.memoryRegion[currentMemoryRegion].getAddress()+self.memoryRegion[currentMemoryRegion].getSize()):
                    indice=0
                    self.rawData[indice]=0x5   #comando di programmazione
                    indice+=1
                    #scriviamo l'indirizzo - il byte più significativo per primo
                    appoggio=address & 0x00FFFF
                    self.rawData[indice]=(appoggio & 0x00FF)
                    indice+=1
                    self.rawData[indice]=(appoggio & 0xFF00)>>8
                    indice+=1
                    appoggio=address & 0xFFFF0000
                    appoggio=appoggio>>16
                    self.rawData[indice]=(appoggio & 0x00FF)
                    indice+=1
                    self.rawData[indice]=(appoggio & 0xFF00)>>8
                    indice+=1
                    self.rawData[indice]=self.bytesPerPacket
                    indice+=1
                
                    for i in range(0,self.bytesPerPacket):
                        dato=self.pData[currentMemoryRegion][contaDati]
                        contaDati+=1
                        self.rawData[indice+i+self.bytesPerPacket+2-self.bytesPerPacket]=dato
                        if dato!=0xFF:
                            if self.bytesPerAddress==2:
                                if (address % 2)!=0:
                                    if currentByteInAddress!=2:
                                        skipBlock=False
                                else:
                                    skipBlock=False
                            else:
                                skipBlock=False
                    
                        if currentByteInAddress==self.bytesPerAddress:
                            address+=1
                            currentByteInAddress=1
                        else:
                            currentByteInAddress+=1
                    
                        if address>=(self.memoryRegion[currentMemoryRegion].getAddress()+self.memoryRegion[currentMemoryRegion].getSize()):
                            i+=1
                            for n in range(0,58):
                                if n<i:
                                    self.rawData[indice+58-n-1]=self.rawData[indice+i+(58-self.bytesPerPacket)-n-1]
                                else:
                                    self.rawData[indice+58-n-1]=0
                        
                            break
                
                    #scrivo i bytes per packet effettivi
                    self.rawData[indice-1]=i+1
                    if (i+1)!=56:
                        i+=1
                        i-=1
                
                    if skipBlock==False:
                        if blockSkipped==True:
                            rawDataTemp=bytearray(64)
                            rawDataTemp[0]=0x06  #comando program complete
                            ris=self.spedisciUSB(dispositivoBoot, rawDataTemp, 1000,len(rawDataTemp))
                            if ris is None:
                                return
                            blockSkipped=False
                        ris=self.spedisciUSB(dispositivoBoot, self.rawData, 1000, len(self.rawData))
                    
                        if ris is None:
                            return
                    
                        skipBlock=True
                    else:
                        blockSkipped=True
                        skipBlock=True
            
                #adesso in ogni caso spedisco il comando program complete
                self.rawData[0]=0x06
                ris=self.spedisciUSB(dispositivoBoot, self.rawData, 1000, len(self.rawData))
                if ris is None:
                    return
            
            if configsProgrammed==False:
                configsProgrammed=True
            else:
                everythingElseProgrammed=True
        
        return 1
    
    def letturaHex(self,dispositivoBoot,idDispositivo,nomeFile):
        #in questa funzione mi collego alla mab e leggo il file HEX
        #per prima cosa devo spedire una query al dispositivo per avere informazioni sullo stesso
        packetsWritten=0
        packetsRead=0
        BytesWritten=0
        BytesReceived=0
        rawDataAnswer=bytearray(64)
        p=0
        pSave=0
        
        self.memoryRegion.clear()
        
        rawDataAnswer=self.spedisciQuery(dispositivoBoot, idDispositivo,1000)
        if rawDataAnswer is None:
            return
        
        self.bytesPerPacket=rawDataAnswer[1]
        self.deviceFamily=rawDataAnswer[2]
    
        contaByte=3
        tipo=0
        indirizzo=0
        dimensione=0
        appoggio=0;
        appoggio1=0
        finito=False
        j=0
        self.memoryRegionsDetected=0
        tmp=0
    
        while finito==False:
            tipo=rawDataAnswer[contaByte]
            if tipo==255:
                #terminata la cosa devo uscire dal ciclo for
                finito=True
                break
            
            #calcolo l'indirizzo della regione
            contaByte+=1
            appoggio=rawDataAnswer[contaByte]
            contaByte+=1
            tmp=rawDataAnswer[contaByte]
            tmp=tmp<<8
            appoggio+=tmp
            contaByte+=1
            appoggio1=rawDataAnswer[contaByte]
            contaByte+=1
            tmp=rawDataAnswer[contaByte]
            tmp=tmp<<8
            appoggio1+=tmp
            contaByte+=1
            appoggio1=appoggio1<<16
            indirizzo=appoggio+appoggio1
            
            #calcolo la dimensione della regione di memoria
            appoggio=rawDataAnswer[contaByte]
            contaByte+=1
            tmp=rawDataAnswer[contaByte]
            tmp=tmp<<8
            appoggio+=tmp
            contaByte+=1
            appoggio1=rawDataAnswer[contaByte]
            contaByte+=1
            tmp=rawDataAnswer[contaByte]
            tmp=tmp<<8
            appoggio1+=tmp
            contaByte+=1
            appoggio1=appoggio1<<16
            dimensione=appoggio+appoggio1
            self.memoryRegion.append(MEMORYREGION(tipo,indirizzo,dimensione))
            self.memoryRegionsDetected+=1
            j+=1
            if j>=6:
                finito=True
    
        #definisco il numero di bytes per address sulla base del tipo di micro
        self.bytesPerAddress=0
        if self.deviceFamily==DEVICE_FAMILY_PIC18:
            self.bytesPerAddress=1
        elif self.deviceFamily==DEVICE_FAMILY_PIC24:
            self.bytesPerAddress=2
        elif self.deviceFamily==DEVICE_FAMILY_PIC32:
            self.bytesPerAddress=3
    
        #for i in range(0,(self.memoryRegion[0].getSize()+1)*self.bytesPerAddress):
        self.pData[0]=bytearray((self.memoryRegion[0].getSize()+1)*self.bytesPerAddress)
            
        if self.memoryRegionsDetected>1:
            #for i in range(0,(self.memoryRegion[1].getSize()+1)*self.bytesPerAddress):
            self.pData[1]=bytearray((self.memoryRegion[1].getSize()+1)*self.bytesPerAddress)
            if self.memoryRegionsDetected>2:
#                for i in range(0,(self.memoryRegion[2].getSize()+1)*self.bytesPerAddress):
                self.pData[2]=bytearray((self.memoryRegion[2].getSize()+1)*self.bytesPerAddress)
                if self.memoryRegionsDetected>3:
#                    for i in range(0,(self.memoryRegion[3].getSize()+1)*self.bytesPerAddress):
                    self.pData[3]=bytearray((self.memoryRegion[3].getSize()+1)*self.bytesPerAddress)
                    if self.memoryRegionsDetected>4:
#                        for i in range(0,(self.memoryRegion[4].getSize()+1)*self.bytesPerAddress):
                        self.pData[4]=bytearray((self.memoryRegion[4].getSize()+1)*self.bytesPerAddress)
                        if self.memoryRegionsDetected>5:
                            
#                            for i in range(0,(self.memoryRegion[5].getSize()+1)*self.bytesPerAddress):
                            self.pData[5]=bytearray((self.memoryRegion[5].getSize()+1)*self.bytesPerAddress)
                
        #adesso ho le informazioni necessarie per andare a leggere la memoria del dispositivo
        for currentMemoryRegion in range(0,self.memoryRegionsDetected):
            if self.memoryRegion[currentMemoryRegion].getType()==3:
                #la regione è la regione dei bit di configurazione che non vengono programmati
                continue
            
            addressToRequest=self.memoryRegion[currentMemoryRegion].getAddress()
            dimensione=self.memoryRegion[currentMemoryRegion].getSize()
            contaByte=0
            contaSave=0
            
            while addressToRequest<(self.memoryRegion[currentMemoryRegion].getAddress()+self.memoryRegion[currentMemoryRegion].getSize()):
                self.rawData[0]=0x07   #comando GET_DATA
                #carico l'indirizzo
                appoggio=addressToRequest & 0x00FFFF
                indice=1
                self.rawData[indice]=(appoggio & 0x00FF)
                indice+=1
                self.rawData[indice]=(appoggio & 0xFF00)>>8
                indice+=1
                appoggio=addressToRequest & 0xFFFF0000
                appoggio=appoggio>>16
                self.rawData[indice]=(appoggio & 0x00FF)
                indice+=1
                self.rawData[indice]=(appoggio & 0xFF00)>>8
                indice+=1
                #carico il numero di byte per packet
                self.rawData[indice]=self.bytesPerPacket
                indice+=1
                
                if (addressToRequest+int(self.bytesPerPacket/self.bytesPerAddress))>(self.memoryRegion[currentMemoryRegion].getAddress()+self.memoryRegion[currentMemoryRegion].getSize()):
                    self.rawData[indice-1]=self.bytesPerPacket-((addressToRequest+int(self.bytesPerPacket/self.bytesPerAddress)-(self.memoryRegion[currentMemoryRegion].getAddress()+self.memoryRegion[currentMemoryRegion].getSize())))*self.bytesPerAddress
                
                while (self.rawData[indice-1] % self.bytesPerAddress)!=0:
                    self.rawData[indice-1]+=1
                
                rit=self.spedisciUSB(dispositivoBoot, self.rawData, 3000, 64)
                if rit is None:
                    return
                
                addressToRequest+=int(self.bytesPerPacket/self.bytesPerAddress)
                
                dispositivoBoot.set_configuration()
                cfg = dispositivoBoot.get_active_configuration()
                intf = cfg[(0,0)]
                ep = usb.util.find_descriptor(
                    intf,
                    # match the first OUT endpoint
                    custom_match = \
                    lambda e: \
                    usb.util.endpoint_direction(e.bEndpointAddress) == \
                    usb.util.ENDPOINT_IN)
        
                try:
                    rawDataAnswer=dispositivoBoot.read(ep,64,2000)
                except:
                    return
                
                if len(rawDataAnswer)!=64:
                    return
                
                #adesso devo andare a leggere il contenuto
                responseAddress=0
                indiceAnswer=1
                appoggio=rawDataAnswer[indiceAnswer]
                indiceAnswer+=1
                tmp=rawDataAnswer[indiceAnswer]
                tmp=tmp<<8
                appoggio+=tmp
                indiceAnswer+=1
                appoggio1=rawDataAnswer[indiceAnswer]
                indiceAnswer+=1
                tmp=rawDataAnswer[indiceAnswer]
                tmp=tmp<<8
                appoggio1+=tmp
                indiceAnswer+=1
                appoggio1=appoggio1<<16
                responseAddress=appoggio+appoggio1
                
                bytePerPacketAnswer=rawDataAnswer[indiceAnswer]
                indiceAnswer+=1
                
                if responseAddress==0 or bytePerPacketAnswer==0 or rawDataAnswer[0]==0:
                    #problema di trasmissione
                    return
                
                packetsRead+=1
                pSave=p+(responseAddress-self.memoryRegion[currentMemoryRegion].getAddress())*self.bytesPerAddress
                for i in range(0,bytePerPacketAnswer):
                    tempData2=rawDataAnswer[indiceAnswer+bytePerPacketAnswer+2-bytePerPacketAnswer+i]
                    self.pData[currentMemoryRegion][pSave]=tempData2
                    pSave+=1
                
                
        
        #a questo punto la memoria letta devo tradurla in un file Hex
        fileHex=open(nomeFile,'wb')
        
        for currentMemoryRegion in range(0,self.memoryRegionsDetected):
            fileHex.write(self.pData[currentMemoryRegion])
        
        fileHex.close()
        
        return 1
    
    def HexToString(self,numero,lenStringa):
        stringaToConvert=""
        
        numByte=(lenStringa*8)-4        
        for i in range(0,lenStringa*2):
            c=(numero >> numByte) & 0x0000000F
            stringaToConvert+="{0:X}".format(c)
            numByte-=4
        
        return stringaToConvert
    
    def convertBinaryToHex(self,nomeFile):
        #questa funzione converte il contenuto del memory region in un file hex
        #questa funzione da' per scontato che in memory region ci sia già qualcosa
        hexFile=open(nomeFile,'w')
        bytesWritten=0
        bytesReceived=0
        Stringa=""
        bytesThisLine=0
        
        for currentMemoryRegion in range(0,self.memoryRegionsDetected):
            address=self.memoryRegion[currentMemoryRegion].getAddress()*self.bytesPerAddress
            size=self.memoryRegion[currentMemoryRegion].getSize()
            p=0
            checksum = 0x01 + ~(0x02 + 0x04 + ((address >> 24) & 0xFF) + ((address >> 16) & 0xFF))
            hexFile.write(Stringa+":02000004"+ self.HexToString((address >> 16), 2)+self.HexToString(checksum, 1)+"\n")
            
            lastVector=(address >> 16) & 0xFFFF
            
            while True:
                thisVector=(address >> 16) & 0xFFFF
                if thisVector!=lastVector:
                    checksum = 0x01 + ~(0x02 + 0x04 + ((address >> 24) & 0xFF) + ((address >> 16) & 0xFF))
                    Stringa=""
                    hexFile.write(":02000004"+ self.HexToString((address >> 16), 2)+self.HexToString(checksum, 1)+"\n")
                checksum=0
                printString=":"
                bytesThisLine=16
                if (address+16)>(self.memoryRegion[currentMemoryRegion].getAddress()+self.memoryRegion[currentMemoryRegion].getSize()):
                    bytesThisLine=16-((address+16)-((self.memoryRegion[currentMemoryRegion].getAddress()+self.memoryRegion[currentMemoryRegion].getSize())*self.bytesPerAddress))
                printString+=self.HexToString(bytesThisLine, 1)
                checksum+=~(bytesThisLine)+1
                printString+=self.HexToString(checksum, 2)
                checksum+=(~(address & 0xFF)+1)
                checksum+=(~((address>>8) & 0xFF)+1)
                printString+="00"
                
                for i in range(0,bytesThisLine):
                    dataByte=self.pData[currentMemoryRegion][p]
                    p+=1
                    printString+=self.HexToString(dataByte, 1)
                    checksum+=(~(dataByte)+1)
                
                printString+=self.HexToString(checksum, 1)
                printString+="\n"
                hexFile.write(printString)
                address+=bytesThisLine
                if address>=(self.memoryRegion[currentMemoryRegion].getAddress()+self.memoryRegion[currentMemoryRegion].getSize())*self.bytesPerAddress:
                    break
                
                lastVector=thisVector
        
        hexFile.write(":00000001FF\n")
        
        hexFile.close()
        return 1    

    def resetDevice(self,dispositivoBoot,idDispositivo):
        #questa funzione serve a resettare un dispositivo
        self.rawData[0]=8
        ret=self.spedisciUSB(dispositivoBoot, self.rawData, 1000, len(self.rawData))
        if ret is None:
            return
        
        return 1

class leggiOperazioni():
    def leggiFile(self,nomeFile):
        f=open(nomeFile)
        data=json.load(f)
        f.close()
        return data

def funzioneMain():
    fileName=".//fileOperazioni.json"
    nomeFileProgram=""
    if os.path.isfile(fileName):
        sampleDevice=programmaFirmware()
        fileJSON=leggiOperazioni()
        datiJSON=fileJSON.leggiFile(fileName)
        if datiJSON is None:
            print("Error - Dati JSON non validi")
            return
        
        bootLoader=sampleDevice.connettiDispositivo()
        if bootLoader is None:
            print("Error - Il bootloader non risponde")
            return
        #adesso abbiamo collegato il bootloader e abbiamo letto il file JSON - andiamo a vedere cosa conteneva
        #il file JSON e eseguiamo le varie operazioni
        chiavi=datiJSON.keys()
        i=0
        for chiave in chiavi:
            i=i+1
            if datiJSON[chiave]["operazione"]=="erase":
                #devo eseguire un erase su il dispositivo
                print("Process - {0} iniziata\n".format(chiave),flush=True)
                rit=sampleDevice.spedisciErase(bootLoader,datiJSON[chiave]["indSeriale"])
                if rit is None:
                    print("Error - processo erase fallito\n")
                    break
                else:
                    print("Process - Operazione {0} terminata con successo\n".format(datiJSON[chiave]["operazione"]),flush=True)
            elif datiJSON[chiave]["operazione"]=="program":
                #devo eseguire programmazione con verifica
                print("Process - {0} iniziata\n".format(chiave),flush=True)
                print("Process - Operazione erase attivata\n",flush=True)
                rit=sampleDevice.spedisciErase(bootLoader, datiJSON[chiave]["indSeriale"])
    
                if rit is None:
                    #c'è stato un errore nell'erase
                    print("Error - operazione erase fallita\n",flush=True)
                    break
                
                ricaricaMem=False
                if nomeFileProgram!=datiJSON[chiave]["nomeFile"]:
                    ricaricaMem=True
                nomeFileProgram=datiJSON[chiave]["nomeFile"]
                print("Process - Operazione program attivata\n",flush=True)
                rit=sampleDevice.gestisciProgrammazione(bootLoader, datiJSON[chiave]["nomeFile"], datiJSON[chiave]["indSeriale"],ricaricaMem)
                if rit is None:
                    print("Error - processo program fallito\n",flush=True)
                    break
                else:
                    #adesso devo verificare la programmazione
                    print("Process - operazione verify attivata\n",flush=True)
                    ret=sampleDevice.verificaProgram(bootLoader, datiJSON[chiave]["indSeriale"])    
                    if ret is None:
                        print("Error - processo verify fallito\n",flush=True)
                        break
                    else:
                        print("Process - Operazione {0} terminata con successo\n".format(datiJSON[chiave]["operazione"]),flush=True)
            elif datiJSON[chiave]["operazione"]=="read":
                #devo leggere il contenuto e salvarlo in un file binario
                print("Process - {0} iniziata\n".format(chiave),flush=True)
                print("Process - lettura memoria attivata\n",flush=True)
                sleep(0.01)
                
                ret=sampleDevice.letturaHex(bootLoader, datiJSON[chiave]["indSeriale"], ".//temp.bin")
#                ret=sampleDevice.letturaHex(bootLoader, datiJSON[chiave]["indSeriale"], datiJSON[chiave]["nomeFile"])
                ret=sampleDevice.convertBinaryToHex(datiJSON[chiave]["nomeFile"])
                if ret is None:
                    print("Error - lettura memoria fallita\n")
                    break
                #adesso creo il file hex
#                convertBinaryToHex(".//temp.bin",datiJSON[chiave]["nomeFile"])
                print("Process - lettura memoria terminata con successo\n",flush=True)
            elif datiJSON[chiave]["operazione"]=="verify":
                #devo verificare il contenuto della memoria rispetto al file passato come parametro
                print("Process - {0} iniziata\n".format(chiave),flush=True)
                print("Process - Operazione di caricamento file hex attivata\n",flush=True)
                ret=sampleDevice.caricaFileHex(bootLoader, datiJSON[chiave]["indSeriale"], datiJSON[chiave]["nomeFile"])
                if ret is None:
                    print("Error - Problema nel caricamento file hex\n",flush=True)
                    break
                print("Process - Operazione di verifica attivata\n",flush=True)
                ret=sampleDevice.verificaProgram(bootLoader,datiJSON[chiave]["indSeriale"])
                if ret is None:
                    print("Error - Problema nella verifica file\n",flush=True)
                    break
                print("Process - Operazione di verifica terminata con successo\n",flush=True)
            elif datiJSON[chiave]["operazione"]=="reset":
                print("Process - {0} iniziata\n".format(chiave),flush=True)
                print("Process - Operazione di reset attivata\n",flush=True)
                ret=sampleDevice.resetDevice(bootLoader, datiJSON[chiave]["indSeriale"])
                if ret is None:
                    print("Error - Problema nel comando di reset\n",flush=True)
                    break
                print("Process - operazione di reset terminata con successo\n",flush=True)
            elif datiJSON[chiave]["operazione"]=="query":
                print("Process - {0} iniziata\n".format(chiave),flush=True)
                print("Process - Operazione di query attivata\n",flush=True)
                ret=sampleDevice.spedisciQuery(bootLoader, datiJSON[chiave]["indSeriale"], 1000)
                if ret is None:
                    print("Error - Problema nel comando di query\n",flush=True)
                print("Process - operazione di query terminata con successo\n",flush=True)
                
            else:
                print("Error - operazione non riconosciuta\n",flush=True)
                break
                
    else:
        print("Error - il file JSON non esiste!\n",flush=True)
    
if __name__ == '__main__':
    funzioneMain()        
        
        
        
        
    




        
        

    