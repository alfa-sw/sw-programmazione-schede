#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    caricaOggetti();
    progSetting=new programSetting(&Programma);
    statoOperazioni=STATO_IDLE;
    sottoStatoOperazioni=0;
    timOperazioni=new QTimer(this);
    connect(timOperazioni,SIGNAL(timeout()),this,SLOT(timOperazioniScaduto()),Qt::AutoConnection);
    timOperazioni->start(50);
    timeoutProcesso=0;
    processoBatch=new QProcess(this);
    envProcesso=QProcessEnvironment::systemEnvironment();
    connect(processoBatch,SIGNAL(errorOccurred(QProcess::ProcessError)),this,SLOT(erroreProcesso(QProcess::ProcessError)),Qt::AutoConnection);
    connect(processoBatch,SIGNAL(started()),this,SLOT(partitoProcesso()),Qt::AutoConnection);
    connect (processoBatch, SIGNAL(readyReadStandardOutput()), this, SLOT(printOutput()));
    connect (processoBatch, SIGNAL(readyReadStandardError()), this, SLOT(printError()));//    processoBatch->setWorkingDirectory(QCoreApplication::applicationDirPath());
    processoBatch->setProcessEnvironment(envProcesso);
    timAttesaProcesso=new QTimer(this);
    connect(timAttesaProcesso,SIGNAL(timeout()),this,SLOT(timProcessoScaduto()),Qt::AutoConnection);
    visualizzaTasti=false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::caricaOggetti()
{
    int i;
    int x,y;
    QString Stringa;

    x=20;
    y=40;

    // adesso impostiamo i check box per i coloranti
    mapAttuatori=new QSignalMapper(this);

    x=10;
    y=20;
    for (i=0;i<32;i++)
    {
        chkAttuatori[i]=new QCheckBox(ui->grpAttuatori);
        Stringa.sprintf("checkAttuatori%d",i+1);
        chkAttuatori[i]->setObjectName(Stringa);
        chkAttuatori[i]->setGeometry(QRect(x,y,71,17));
        Stringa.sprintf("Pompa %d",i+1);
        chkAttuatori[i]->setText(Stringa);
        chkAttuatori[i]->setEnabled(true);
        chkAttuatori[i]->setVisible(true);
        connect(chkAttuatori[i],SIGNAL(stateChanged(int)),mapAttuatori,SLOT(map()),Qt::AutoConnection);
        mapAttuatori->setMapping(chkAttuatori[i],i);

        if ((((i+1)%8)==0) && (i>0))
        {
            x=10;
            y+=20;
        }
        else
        {
            x+=80;
        }
    }

    connect(mapAttuatori,SIGNAL(mapped(int)),this,SLOT(chkAttuatoriStateChanged(int)));

}

void MainWindow::chkAttuatoriStateChanged(int indice)
{
    // qui ci passo quando clicco sul check dei colori
    QString Stringa;
    QString Stringa1;

    Stringa=ui->txtStato->toPlainText();
    if (chkAttuatori[indice]->checkState()==Qt::Checked)
    {
        Stringa1.sprintf("Aggiunto attuatore %d\n",indice+1);
        Programma.programmaAttuatori[indice]=true;
    }
    else
    {
        Stringa1.sprintf("Eliminato attuatore %d\n",indice+1);
        Programma.programmaAttuatori[indice]=false;
    }
    Stringa+=Stringa1;
    ui->txtStato->setText(Stringa);
    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::on_btnSelezionaHexMAB_clicked()
{
    if (caricaFile("Apri programma MAB",&Programma.nomeFileMAB,"per MAB"))
    {
        ui->btnSelezionaHexMAB->setStyleSheet("background-color: green");
    }
    else
    {
        ui->btnSelezionaHexMAB->setStyleSheet("background-color: rgb(225,225,225,255)");
    }
}

int MainWindow::caricaFile(QString stringaTitolo, QString *stringaFile,QString Attuatore)
{
    QFileDialog *fileDialog;
    QStringList nomeFile;
    QString Stringa;

    fileDialog=new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setNameFilter("File Hex (*.hex)");
    fileDialog->setViewMode(QFileDialog::Detail);
    fileDialog->setWindowTitle(stringaTitolo);
    if (fileDialog->exec())
    {
        // ha premuto ok - andiamo a vedere cosa ha scelto
        nomeFile=fileDialog->selectedFiles();
        Stringa=ui->txtStato->toPlainText();
        Stringa+="Caricato file "+nomeFile[0]+" " +Attuatore+"\n";
        ui->txtStato->setText(Stringa);
        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
        *stringaFile=nomeFile[0];
        return 1;
    }

    return 0;
}

void MainWindow::on_btnAnnullaSelezioneMAB_clicked()
{
    ui->chkMAB->setCheckState(Qt::Unchecked);
    Programma.programmaMAB=false;

    Programma.nomeFileMAB="";
    ui->btnSelezionaHexMAB->setStyleSheet("background-color: rgb(225,225,225,255)");
}

void MainWindow::on_chkMAB_stateChanged(int arg1)
{
    // qui qualcuno ha cliccato sul check box della mab - dobbiamo vedere se seleziona o meno la MAB
//    arg1=1;
    QString Stringa;
    QString Stringa1;

    Stringa=ui->txtStato->toPlainText();
    if (ui->chkMAB->checkState()==Qt::Checked)
    {
        Stringa1.sprintf("Aggiunta MAB\n");
        Programma.programmaMAB=true;
    }
    else
    {
        Stringa1.sprintf("Eliminata MAB\n");
        Programma.programmaMAB=false;
    }
    Stringa+=Stringa1;
    ui->txtStato->setText(Stringa);
    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::on_chkAsseX_stateChanged(int arg1)
{
    QString Stringa;
    QString Stringa1;

    Stringa=ui->txtStato->toPlainText();
    if (ui->chkAsseX->checkState()==Qt::Checked)
    {
        Stringa1.sprintf("Aggiunta asse X\n");
        Programma.programmaAssi[0]=true;
    }
    else
    {
        Stringa1.sprintf("Eliminata asse X\n");
        Programma.programmaAssi[0]=false;
    }
    Stringa+=Stringa1;
    ui->txtStato->setText(Stringa);
    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::on_chkAsseY_stateChanged(int arg1)
{
    QString Stringa;
    QString Stringa1;

    Stringa=ui->txtStato->toPlainText();
    if (ui->chkAsseY->checkState()==Qt::Checked)
    {
        Stringa1.sprintf("Aggiunta asse Y\n");
        Programma.programmaAssi[1]=true;
    }
    else
    {
        Stringa1.sprintf("Eliminata asse Y\n");
        Programma.programmaAssi[1]=false;
    }
    Stringa+=Stringa1;
    ui->txtStato->setText(Stringa);
    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::on_btnSelezionaHexAssi_clicked()
{
    if (caricaFile("Apri programma assi",&Programma.nomeFileAssi,"per Assi"))
    {
        ui->btnSelezionaHexAssi->setStyleSheet("background-color: green");
    }
    else
    {
        ui->btnSelezionaHexAssi->setStyleSheet("background-color: rgb(225,225,225,255)");
    }
}

void MainWindow::on_btnAnnullaSelezioneAssi_clicked()
{
    int i;

    ui->chkAsseX->setCheckState(Qt::Unchecked);
    ui->chkAsseY->setCheckState(Qt::Unchecked);

    for (i=0;i<2;i++)
    {
        Programma.programmaAssi[i]=false;
    }

    Programma.nomeFileAssi="";
    ui->btnSelezionaHexAssi->setStyleSheet("background-color: rgb(225,225,225,255)");

}

void MainWindow::on_chkCover1_stateChanged(int arg1)
{
    QString Stringa;
    QString Stringa1;

    Stringa=ui->txtStato->toPlainText();
    if (ui->chkCover1->checkState()==Qt::Checked)
    {
        Stringa1.sprintf("Aggiunto Cover 1\n");
        Programma.programmaCover[0]=true;
    }
    else
    {
        Stringa1.sprintf("Eliminato Cover 1\n");
        Programma.programmaCover[0]=false;
    }
    Stringa+=Stringa1;
    ui->txtStato->setText(Stringa);
    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::on_chkCover2_stateChanged(int arg1)
{
    QString Stringa;
    QString Stringa1;

    Stringa=ui->txtStato->toPlainText();
    if (ui->chkCover2->checkState()==Qt::Checked)
    {
        Stringa1.sprintf("Aggiunto Cover 2\n");
        Programma.programmaCover[1]=true;
    }
    else
    {
        Stringa1.sprintf("Eliminato Cover 2\n");
        Programma.programmaCover[1]=false;
    }
    Stringa+=Stringa1;
    ui->txtStato->setText(Stringa);
    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::on_btnSelezionaHexCover_clicked()
{
    if (caricaFile("Apri programma cover",&Programma.nomeFileCover,"per Cover"))
    {
        ui->btnSelezionaHexCover->setStyleSheet("background-color: green");
    }
    else
    {
        ui->btnSelezionaHexCover->setStyleSheet("background-color: rgb(225,225,225,255)");
    }

}

void MainWindow::on_btnAnnullaSelezioneCover_clicked()
{
    int i;

    ui->chkCover1->setCheckState(Qt::Unchecked);
    ui->chkCover2->setCheckState(Qt::Unchecked);

    for (i=0;i<2;i++)
    {
        Programma.programmaCover[i]=false;
    }

    Programma.nomeFileCover="";
    ui->btnSelezionaHexCover->setStyleSheet("background-color: rgb(225,225,225,255)");

}

void MainWindow::on_btnSelezionaHexAttuatori_clicked()
{
    // devo selezionare il file corretto
    if (caricaFile("Apri programma attuatori",&Programma.nomeFileAttuatori,"per Pompe"))
    {
        ui->btnSelezionaHexAttuatori->setStyleSheet("background-color: green");
    }
    else
    {
        ui->btnSelezionaHexAttuatori->setStyleSheet("background-color: rgb(225,225,225,255)");
    }

}

void MainWindow::on_btnAnnullaSelezioneAttuatori_clicked()
{
    int i;

    for (i=0;i<32;i++)
    {
        chkAttuatori[i]->setCheckState(Qt::Unchecked);
        Programma.programmaAttuatori[i]=false;
    }

    Programma.nomeFileAttuatori="";
    ui->btnSelezionaHexAttuatori->setStyleSheet("background-color: rgb(225,225,225,255)");

}

void MainWindow::on_chkContainer1_stateChanged(int arg1)
{
    QString Stringa;
    QString Stringa1;

    Stringa=ui->txtStato->toPlainText();
    if (ui->chkContainer1->checkState()==Qt::Checked)
    {
        Stringa1.sprintf("Aggiunto Container 1\n");
        Programma.programmaContainer[0]=true;
    }
    else
    {
        Stringa1.sprintf("Eliminato Container 1\n");
        Programma.programmaContainer[0]=false;
    }
    Stringa+=Stringa1;
    ui->txtStato->setText(Stringa);
    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::on_chkContainer2_stateChanged(int arg1)
{
    QString Stringa;
    QString Stringa1;

    Stringa=ui->txtStato->toPlainText();
    if (ui->chkContainer2->checkState()==Qt::Checked)
    {
        Stringa1.sprintf("Aggiunto Container 2\n");
        Programma.programmaContainer[1]=true;
    }
    else
    {
        Stringa1.sprintf("Eliminato Container 2\n");
        Programma.programmaContainer[2]=false;
    }
    Stringa+=Stringa1;
    ui->txtStato->setText(Stringa);
    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::on_chkContainer3_stateChanged(int arg1)
{
    QString Stringa;
    QString Stringa1;

    Stringa=ui->txtStato->toPlainText();
    if (ui->chkContainer3->checkState()==Qt::Checked)
    {
        Stringa1.sprintf("Aggiunto Container 3\n");
        Programma.programmaContainer[2]=true;
    }
    else
    {
        Stringa1.sprintf("Eliminato Container 3\n");
        Programma.programmaContainer[2]=false;
    }
    Stringa+=Stringa1;
    ui->txtStato->setText(Stringa);
    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::on_chkContainer4_stateChanged(int arg1)
{
    QString Stringa;
    QString Stringa1;

    Stringa=ui->txtStato->toPlainText();
    if (ui->chkContainer4->checkState()==Qt::Checked)
    {
        Stringa1.sprintf("Aggiunto Container 4\n");
        Programma.programmaContainer[3]=true;
    }
    else
    {
        Stringa1.sprintf("Eliminato Container 4\n");
        Programma.programmaContainer[3]=false;
    }
    Stringa+=Stringa1;
    ui->txtStato->setText(Stringa);
    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::on_btnSelezionaHexContainer_clicked()
{
    if (caricaFile("Apri programma container",&Programma.nomeFileContainer,"per Container"))
    {
        ui->btnSelezionaHexContainer->setStyleSheet("background-color: green");
    }
    else
    {
        ui->btnSelezionaHexContainer->setStyleSheet("background-color: rgb(225,225,225,255)");
    }

}

void MainWindow::on_btnAnnullaSelezioneContainer_clicked()
{
    int i;

    ui->chkContainer1->setCheckState(Qt::Unchecked);
    ui->chkContainer2->setCheckState(Qt::Unchecked);
    ui->chkContainer3->setCheckState(Qt::Unchecked);
    ui->chkContainer4->setCheckState(Qt::Unchecked);

    for (i=0;i<4;i++)
    {
        Programma.programmaContainer[i]=false;
    }

    Programma.nomeFileContainer="";
    ui->btnSelezionaHexContainer->setStyleSheet("background-color: rgb(225,225,225,255)");

}

void MainWindow::on_chkAutocap_stateChanged(int arg1)
{
    QString Stringa;
    QString Stringa1;

    Stringa=ui->txtStato->toPlainText();
    if (ui->chkAutocap->checkState()==Qt::Checked)
    {
        Stringa1.sprintf("Aggiunto Autocap\n");
        Programma.programmaAutocap=true;
    }
    else
    {
        Stringa1.sprintf("Eliminato Autocap\n");
        Programma.programmaAutocap=false;
    }
    Stringa+=Stringa1;
    ui->txtStato->setText(Stringa);
    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::on_btnSelezionaHexAutocap_clicked()
{
    if (caricaFile("Apri programma autocap",&Programma.nomeFileAutocap,"per Autocap"))
    {
        ui->btnSelezionaHexAutocap->setStyleSheet("background-color: green");
    }
    else
    {
        ui->btnSelezionaHexAutocap->setStyleSheet("background-color: rgb(225,225,225,255)");
    }

}

void MainWindow::on_btnAnnullaSelezioneAutocap_clicked()
{
    ui->chkAutocap->setCheckState(Qt::Unchecked);

    Programma.programmaAutocap=false;

    Programma.nomeFileAutocap="";
    ui->btnSelezionaHexAutocap->setStyleSheet("background-color: rgb(225,225,225,255)");

}

void MainWindow::on_chkCanLifter_stateChanged(int arg1)
{
    QString Stringa;
    QString Stringa1;

    Stringa=ui->txtStato->toPlainText();
    if (ui->chkCanLifter->checkState()==Qt::Checked)
    {
        Stringa1.sprintf("Aggiunto Can lifter\n");
        Programma.programmaCanLifter=true;
    }
    else
    {
        Stringa1.sprintf("Eliminato Can lifter\n");
        Programma.programmaCanLifter=false;
    }
    Stringa+=Stringa1;
    ui->txtStato->setText(Stringa);
    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::on_btnSelezionaHexCanLifter_clicked()
{
    if (caricaFile("Apri programma can lifter",&Programma.nomeFileCanLifter,"per Can lifter"))
    {
        ui->btnSelezionaHexCanLifter->setStyleSheet("background-color: green");
    }
    else
    {
        ui->btnSelezionaHexCanLifter->setStyleSheet("background-color: rgb(225,225,225,255)");
    }

}

void MainWindow::on_btnAnnullaSelezioneCanLifter_clicked()
{
    ui->chkCanLifter->setCheckState(Qt::Unchecked);

    Programma.programmaCanLifter=false;

    Programma.nomeFileCanLifter="";
    ui->btnSelezionaHexCanLifter->setStyleSheet("background-color: rgb(225,225,225,255)");

}

void MainWindow::on_chkHumidifier_stateChanged(int arg1)
{
    QString Stringa;
    QString Stringa1;

    Stringa=ui->txtStato->toPlainText();
    if (ui->chkHumidifier->checkState()==Qt::Checked)
    {
        Stringa1.sprintf("Aggiunto humidifier\n");
        Programma.programmaHumidifier=true;
    }
    else
    {
        Stringa1.sprintf("Eliminato humidifier\n");
        Programma.programmaHumidifier=false;
    }
    Stringa+=Stringa1;
    ui->txtStato->setText(Stringa);
    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::on_btnSelezionaHexHumidifier_clicked()
{
    if (caricaFile("Apri programma humidifier",&Programma.nomeFileHumidifier,"per Humidifier"))
    {
        ui->btnSelezionaHexHumidifier->setStyleSheet("background-color: green");
    }
    else
    {
        ui->btnSelezionaHexHumidifier->setStyleSheet("background-color: rgb(225,225,225,255)");
    }
}

void MainWindow::on_btnAnnullaSelezioneHumidifier_clicked()
{
    ui->chkHumidifier->setCheckState(Qt::Unchecked);

    Programma.programmaHumidifier=false;

    Programma.nomeFileHumidifier="";
    ui->btnSelezionaHexHumidifier->setStyleSheet("background-color: rgb(225,225,225,255)");

}


void MainWindow::on_btnSelezionaTuttoMAB_clicked()
{
    // seleziono la MAB
    ui->chkMAB->setCheckState(Qt::Checked);
}

void MainWindow::on_btnSelezionaTuttoAssi_clicked()
{
    ui->chkAsseX->setCheckState(Qt::Checked);
    ui->chkAsseY->setCheckState(Qt::Checked);
}

void MainWindow::on_btnSelezionaTuttoCover_clicked()
{
    ui->chkCover1->setCheckState(Qt::Checked);
    ui->chkCover2->setCheckState(Qt::Checked);
}

void MainWindow::on_btnSelezionaTuttoContainer_clicked()
{
    ui->chkContainer1->setCheckState(Qt::Checked);
    ui->chkContainer2->setCheckState(Qt::Checked);
    ui->chkContainer3->setCheckState(Qt::Checked);
    ui->chkContainer4->setCheckState(Qt::Checked);
}


void MainWindow::on_btnSelezionaTuttoAutocap_clicked()
{
    ui->chkAutocap->setCheckState(Qt::Checked);
}

void MainWindow::on_btnSelezionaTuttoAttuatori_clicked()
{
    for (int i=0;i<32;i++)
    {
        chkAttuatori[i]->setCheckState(Qt::Checked);
    }
}


void MainWindow::on_btnSelezionaTuttoCanLifter_clicked()
{
    ui->chkCanLifter->setCheckState(Qt::Checked);
}


void MainWindow::on_btnSelezionaTuttoHumidifier_clicked()
{
    ui->chkHumidifier->setCheckState(Qt::Checked);
}

void MainWindow::on_btnErase_clicked()
{
    // dobbiamo creare il file json per far partire il reset
    if (!dispositivoSel())
    {
        // non è stato selezionato nessun dispositivo - non faccio nulla
        QMessageBox msgBox;

        msgBox.setText("Non è stato selezionato nessun dispositivo!!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        return;
    }

    // ci sono dei dispositivi selezionati procedo con il processo di erase
    ui->btnDumpMemory->setEnabled(false);
    ui->btnErase->setEnabled(false);
    ui->btnExportHex->setEnabled(false);
    ui->btnProgramVerify->setEnabled(false);
    ui->btnQuery->setEnabled(false);
    ui->btnReadDevices->setEnabled(false);
    ui->btnResetDevice->setEnabled(false);
    ui->btnVerify->setEnabled(false);

    statoOperazioni=STATO_ERASE;
    sottoStatoOperazioni=0;

    spegniCheckBox();

}

void MainWindow::spegniCheckBox()
{
    ui->chkMAB->setStyleSheet("color: rgb(0, 0, 0)");
    for (int i=0;i<32;i++)
    {
        this->chkAttuatori[i]->setStyleSheet("color: rgb(0, 0, 0)");
    }

    ui->chkAsseX->setStyleSheet("color: rgb(0, 0, 0)");
    ui->chkAsseY->setStyleSheet("color: rgb(0, 0, 0)");
    ui->chkContainer1->setStyleSheet("color: rgb(0, 0, 0)");
    ui->chkContainer2->setStyleSheet("color: rgb(0, 0, 0)");
    ui->chkContainer3->setStyleSheet("color: rgb(0, 0, 0)");
    ui->chkContainer4->setStyleSheet("color: rgb(0, 0, 0)");
    ui->chkCover1->setStyleSheet("color: rgb(0, 0, 0)");
    ui->chkCover2->setStyleSheet("color: rgb(0, 0, 0)");
    ui->chkAutocap->setStyleSheet("color: rgb(0, 0, 0)");
    ui->chkCanLifter->setStyleSheet("color: rgb(0, 0, 0)");
    ui->chkHumidifier->setStyleSheet("color: rgb(0, 0, 0)");

}

bool MainWindow::dispositivoSel()
{
    bool dispositivoSelezionato=false;
    int i;
    int indice=0;

    for (i=0;i<64;i++)
    {
        strutturaDispositivo[i]=nullptr;
    }

    if (Programma.programmaMAB)
    {
        dispositivoSelezionato=true;
        strutturaDispositivo[indice]=new STRUTTURA_DISPOSITIVO;
        strutturaDispositivo[indice]->IDdispositivo=0xFF;
        strutturaDispositivo[indice]->nomeFile=Programma.nomeFileMAB;
        indice++;
    }

    for (i=0;i<2;i++)
    {
        if (Programma.programmaAssi[i])
        {
            dispositivoSelezionato=true;
            strutturaDispositivo[indice]=new STRUTTURA_DISPOSITIVO;
            strutturaDispositivo[indice]->IDdispositivo=0x21+i;
            strutturaDispositivo[indice]->nomeFile=Programma.nomeFileAssi;
            indice++;
        }
    }

    for (i=0;i<2;i++)
    {
        if (Programma.programmaCover[i])
        {
            strutturaDispositivo[indice]=new STRUTTURA_DISPOSITIVO;
            strutturaDispositivo[indice]->IDdispositivo=0x27+i;
            strutturaDispositivo[indice]->nomeFile=Programma.nomeFileCover;
            indice++;
            dispositivoSelezionato=true;
        }
    }

    for (i=0;i<32;i++)
    {
        if (Programma.programmaAttuatori[i])
        {
            strutturaDispositivo[indice]=new STRUTTURA_DISPOSITIVO;
            strutturaDispositivo[indice]->IDdispositivo=i+1;
            strutturaDispositivo[indice]->nomeFile=Programma.nomeFileAttuatori;
            indice++;
            dispositivoSelezionato=true;
        }
    }

    for (i=0;i<4;i++)
    {
        if (Programma.programmaContainer[i])
        {
            strutturaDispositivo[indice]=new STRUTTURA_DISPOSITIVO;
            strutturaDispositivo[indice]->IDdispositivo=0x23+i;
            strutturaDispositivo[indice]->nomeFile=Programma.nomeFileContainer;
            indice++;
            dispositivoSelezionato=true;
        }
    }

    if (Programma.programmaAutocap)
    {
        strutturaDispositivo[indice]=new STRUTTURA_DISPOSITIVO;
        strutturaDispositivo[indice]->IDdispositivo=0x29;
        strutturaDispositivo[indice]->nomeFile=Programma.nomeFileAutocap;
        indice++;
        dispositivoSelezionato=true;
    }

    if (Programma.programmaCanLifter)
    {
        strutturaDispositivo[indice]=new STRUTTURA_DISPOSITIVO;
        strutturaDispositivo[indice]->IDdispositivo=0x2A;
        strutturaDispositivo[indice]->nomeFile=Programma.nomeFileCanLifter;
        indice++;
        dispositivoSelezionato=true;
    }

    if (Programma.programmaHumidifier)
    {
        strutturaDispositivo[indice]=new STRUTTURA_DISPOSITIVO;
        strutturaDispositivo[indice]->IDdispositivo=0x2B;
        strutturaDispositivo[indice]->nomeFile=Programma.nomeFileHumidifier;
        indice++;
        dispositivoSelezionato=true;
    }

    return dispositivoSelezionato;
}

void MainWindow::timOperazioniScaduto()
{
    timeoutProcesso+=50;

    switch (statoOperazioni)
    {
    case STATO_IDLE:
        // in questo stato sono a riposo - metto a 0 la barra
        ui->progressBar->setValue(0);
        ui->btnProgramVerify->setEnabled(true);
        if (visualizzaTasti)
        {
            ui->btnDumpMemory->setEnabled(false);
            ui->btnErase->setEnabled(true);
            ui->btnExportHex->setEnabled(true);
            ui->btnQuery->setEnabled(true);
            ui->btnReadDevices->setEnabled(true);
            ui->btnResetDevice->setEnabled(true);
            ui->btnVerify->setEnabled(true);
        }
        else
        {
            ui->btnDumpMemory->setEnabled(false);
            ui->btnErase->setEnabled(false);
            ui->btnExportHex->setEnabled(false);
            ui->btnQuery->setEnabled(false);
            ui->btnReadDevices->setEnabled(false);
            ui->btnResetDevice->setEnabled(false);
            ui->btnVerify->setEnabled(false);
        }
        sottoStatoOperazioni=0;
        indiceStringheStdOut=0;

        break;
    case STATO_ERASE:
        gestisciStatoErase();
        break;
    case STATO_PROGRAM:
        gestisciStatoProgram();
        break;
    case STATO_READ_DEVICES:
        gestisciReadDevices();
        break;
    case STATO_EXPORT_HEX:
//        gestisciExportHex();
        break;
    case STATO_VERIFY:
        gestisciVerify();
        break;
    case STATO_RESET:
        gestisciReset();
        break;
    case STATO_QUERY:
        gestisciQuery();
        break;
    default:
        statoOperazioni=STATO_IDLE;
        break;
    }

}

void MainWindow::gestisciReset()
{
    int i;
    int numDevices;
    QString Stringa;
    QString Stringa1;
    QString nomeFileJson;
    QString pathApp = QCoreApplication::applicationDirPath();
    QString NomeFile = "leggiFilebatch.bat";
    QStringList arguments;
    QFile *fileJson;
    QString stringaJson;
    QJsonDocument *documentoJson;

    // in questa funione devo gestire la sequenza di operazioni per lo stato di erase - devo creare il file json con i
    // diversi slave su cui effettuare l'erase
    switch (sottoStatoOperazioni)
    {
    case 0:
        // in questo sottostato devo inizializzare la barra di scorrimento per avere un'idea di quando deve scorrere
        numDevices=0;
        i=0;
        while (strutturaDispositivo[i]!=nullptr)
        {
            numDevices++;
            i++;
        }
        incrementoBarra=100.0/(float)(numDevices);
        valoreBarra=0;
        ui->progressBar->setValue((int)valoreBarra);
        sottoStatoOperazioni=1;
        timeoutProcesso=0;
        dispositivoAttivo=0;
        break;
    case 1:
        // per adesso faccio finta che ci siano i dispositivi da cancellare
        // qui costruisco il file JSON e lancio il file batch
        // per prima cosa costruisco il file json - il processo python si aspetta di trovare il file json nella stessa
        // directory in cui si trova lui
        nomeFileJson=QApplication::applicationDirPath();
        nomeFileJson+="/fileOperazioni.json";
        i=0;
        while (strutturaDispositivo[i]!=nullptr)
        {
            // costruisco gli oggetti JSON
            oggettoInserito["operazione"]="reset";
            oggettoInserito["indSeriale"]=strutturaDispositivo[i]->IDdispositivo;
            oggettoInserito["nomeFile"]=strutturaDispositivo[i]->nomeFile;
            Stringa="operazione"+ Stringa1.sprintf("%d",i+1);
            i++;
            oggettoRoot.insert(Stringa,oggettoInserito);
        }
        numeroOggettiJSON=i;

        // in oggetto root ho tutte le operazioni di erase da fare
        documentoJson= new QJsonDocument(oggettoRoot);

        stringaJson=documentoJson->toJson();
        fileJson=new QFile(nomeFileJson);
        if (!fileJson->open(QIODevice::WriteOnly))
        {
            // non sono riuscito ad aprire il file, cacca!!
            QMessageBox msgbox;
            msgbox.setText("Non sono riuscito ad aprire il file JSON!!!!");
            msgbox.setIcon(QMessageBox::Critical);
            msgbox.exec();
            statoOperazioni=STATO_IDLE;
            sottoStatoOperazioni=0;
            break;
        }

        fileJson->write(stringaJson.toLocal8Bit());
        fileJson->close();

        // ho creato il file JSON adesso devo lanciare il processo batch
        pathApp+="/"+NomeFile;
        arguments << "/c" << pathApp;
        pathApp="cmd.exe";
        processoBatch->setWorkingDirectory(QCoreApplication::applicationDirPath());
        processoBatch->start(pathApp,arguments,QProcess::ReadWrite);
        sottoStatoOperazioni=2;
        batchFinito=false;
        break;
    case 2:
        // qui aspetto che il processo batch mi dica di avere finito
        if (batchFinito)
        {
            Stringa=ui->txtStato->toPlainText();
            Stringa1.sprintf("Fase di reset terminata\n");
            Stringa+=Stringa1;
            ui->txtStato->setText(Stringa);

            ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
            statoOperazioni=STATO_IDLE;
            // devo cancellare tutti gli oggetti
            for (i=1;i<=numeroOggettiJSON;i++)
            {
                Stringa="operazione"+Stringa1.sprintf("%d",i);
                oggettoRoot.remove(Stringa);
            }
            break;
        }

        break;

    default:
        sottoStatoOperazioni=0;
        break;
    }

}

void MainWindow::gestisciReadDevices()
{
    int i;
    int numDevices;
    QString Stringa;
    QString Stringa1;
    QString nomeFileJson;
    QString pathApp = QCoreApplication::applicationDirPath();
    QString NomeFile = "leggiFilebatch.bat";
    QStringList arguments;
    QFile *fileJson;
    QString stringaJson;
    QJsonDocument *documentoJson;

    // in questa funione devo gestire la sequenza di operazioni per lo stato di erase - devo creare il file json con i
    // diversi slave su cui effettuare l'erase
    switch (sottoStatoOperazioni)
    {
    case 0:
        // in questo sottostato devo inizializzare la barra di scorrimento per avere un'idea di quando deve scorrere
        numDevices=0;
        i=0;
        while (strutturaDispositivo[i]!=nullptr)
        {
            numDevices++;
            i++;
        }
        incrementoBarra=100.0/(float)(numDevices);    // ogni operazione di program prevede 3 fasi
        valoreBarra=0;
        ui->progressBar->setValue((int)valoreBarra);
        sottoStatoOperazioni=1;
        timeoutProcesso=0;
        dispositivoAttivo=0;
        break;
    case 1:
        // per adesso faccio finta che ci siano i dispositivi da cancellare
        // qui costruisco il file JSON e lancio il file batch
        // per prima cosa costruisco il file json - il processo python si aspetta di trovare il file json nella stessa
        // directory in cui si trova lui
        nomeFileJson=QApplication::applicationDirPath();
        nomeFileJson+="/fileOperazioni.json";
        i=0;
        while (strutturaDispositivo[i]!=nullptr)
        {
            // costruisco gli oggetti JSON
            oggettoInserito["operazione"]="read";
            oggettoInserito["indSeriale"]=strutturaDispositivo[i]->IDdispositivo;
            oggettoInserito["nomeFile"]=strutturaDispositivo[i]->nomeFile;
            Stringa="operazione"+ Stringa1.sprintf("%d",i+1);
            i++;
            oggettoRoot.insert(Stringa,oggettoInserito);
        }
        numeroOggettiJSON=i;

        // in oggetto root ho tutte le operazioni di erase da fare
        documentoJson= new QJsonDocument(oggettoRoot);

        stringaJson=documentoJson->toJson();
        fileJson=new QFile(nomeFileJson);
        if (!fileJson->open(QIODevice::WriteOnly))
        {
            // non sono riuscito ad aprire il file, cacca!!
            QMessageBox msgbox;
            msgbox.setText("Non sono riuscito ad aprire il file JSON!!!!");
            msgbox.setIcon(QMessageBox::Critical);
            msgbox.exec();
            statoOperazioni=STATO_IDLE;
            sottoStatoOperazioni=0;
            break;
        }

        fileJson->write(stringaJson.toLocal8Bit());
        fileJson->close();

        // ho creato il file JSON adesso devo lanciare il processo batch
        pathApp+="/"+NomeFile;
        arguments << "/c" << pathApp;
        pathApp="cmd.exe";
        processoBatch->setWorkingDirectory(QCoreApplication::applicationDirPath());
        processoBatch->start(pathApp,arguments,QProcess::ReadWrite);
        sottoStatoOperazioni=2;
        batchFinito=false;
        break;
    case 2:
        // qui aspetto che il processo batch mi dica di avere finito
        if (batchFinito)
        {
            Stringa=ui->txtStato->toPlainText();
            Stringa1.sprintf("Fase di lettura terminata\n");
            Stringa+=Stringa1;
            ui->txtStato->setText(Stringa);

            ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
            statoOperazioni=STATO_IDLE;
            // devo cancellare tutti gli oggetti
            for (i=1;i<=numeroOggettiJSON;i++)
            {
                Stringa="operazione"+Stringa1.sprintf("%d",i);
                oggettoRoot.remove(Stringa);
            }
            break;
        }

        break;

    default:
        sottoStatoOperazioni=0;
        break;
    }
}


void MainWindow::gestisciQuery()
{
    int i;
    int numDevices;
    QString Stringa;
    QString Stringa1;
    QString nomeFileJson;
    QString pathApp = QCoreApplication::applicationDirPath();
    QString NomeFile = "leggiFilebatch.bat";
    QStringList arguments;
    QFile *fileJson;
    QString stringaJson;
    QJsonDocument *documentoJson;

    // in questa funione devo gestire la sequenza di operazioni per lo stato di erase - devo creare il file json con i
    // diversi slave su cui effettuare l'erase
    switch (sottoStatoOperazioni)
    {
    case 0:
        // in questo sottostato devo inizializzare la barra di scorrimento per avere un'idea di quando deve scorrere
        numDevices=0;
        i=0;
        while (strutturaDispositivo[i]!=nullptr)
        {
            numDevices++;
            i++;
        }
        incrementoBarra=100.0/(float)(numDevices);    // ogni operazione di program prevede 3 fasi
        valoreBarra=0;
        ui->progressBar->setValue((int)valoreBarra);
        sottoStatoOperazioni=1;
        timeoutProcesso=0;
        dispositivoAttivo=0;
        break;
    case 1:
        // per adesso faccio finta che ci siano i dispositivi da cancellare
        // qui costruisco il file JSON e lancio il file batch
        // per prima cosa costruisco il file json - il processo python si aspetta di trovare il file json nella stessa
        // directory in cui si trova lui
        nomeFileJson=QApplication::applicationDirPath();
        nomeFileJson+="/fileOperazioni.json";
        i=0;
        while (strutturaDispositivo[i]!=nullptr)
        {
            // costruisco gli oggetti JSON
            oggettoInserito["operazione"]="query";
            oggettoInserito["indSeriale"]=strutturaDispositivo[i]->IDdispositivo;
            oggettoInserito["nomeFile"]=strutturaDispositivo[i]->nomeFile;
            Stringa="operazione"+ Stringa1.sprintf("%d",i+1);
            i++;
            oggettoRoot.insert(Stringa,oggettoInserito);
        }
        numeroOggettiJSON=i;

        // in oggetto root ho tutte le operazioni di erase da fare
        documentoJson= new QJsonDocument(oggettoRoot);

        stringaJson=documentoJson->toJson();
        fileJson=new QFile(nomeFileJson);
        if (!fileJson->open(QIODevice::WriteOnly))
        {
            // non sono riuscito ad aprire il file, cacca!!
            QMessageBox msgbox;
            msgbox.setText("Non sono riuscito ad aprire il file JSON!!!!");
            msgbox.setIcon(QMessageBox::Critical);
            msgbox.exec();
            statoOperazioni=STATO_IDLE;
            sottoStatoOperazioni=0;
            break;
        }

        fileJson->write(stringaJson.toLocal8Bit());
        fileJson->close();

        // ho creato il file JSON adesso devo lanciare il processo batch
        pathApp+="/"+NomeFile;
        arguments << "/c" << pathApp;
        pathApp="cmd.exe";
        processoBatch->setWorkingDirectory(QCoreApplication::applicationDirPath());
        processoBatch->start(pathApp,arguments,QProcess::ReadWrite);
        sottoStatoOperazioni=2;
        batchFinito=false;
        break;
    case 2:
        // qui aspetto che il processo batch mi dica di avere finito
        if (batchFinito)
        {
            Stringa=ui->txtStato->toPlainText();
            Stringa1.sprintf("Fase di query terminata\n");
            Stringa+=Stringa1;
            ui->txtStato->setText(Stringa);

            ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
            statoOperazioni=STATO_IDLE;

            // devo cancellare tutti gli oggetti
            for (i=1;i<=numeroOggettiJSON;i++)
            {
                Stringa="operazione"+Stringa1.sprintf("%d",i);
                oggettoRoot.remove(Stringa);
            }
            break;
        }

        break;

    default:
        sottoStatoOperazioni=0;
        break;
    }
}

void MainWindow::gestisciVerify()
{
    int i;
    int numDevices;
    QString Stringa;
    QString Stringa1;
    QString nomeFileJson;
    QString pathApp = QCoreApplication::applicationDirPath();
    QString NomeFile = "leggiFilebatch.bat";
    QStringList arguments;
    QFile *fileJson;
    QString stringaJson;
    QJsonDocument *documentoJson;

    // in questa funione devo gestire la sequenza di operazioni per lo stato di erase - devo creare il file json con i
    // diversi slave su cui effettuare l'erase
    switch (sottoStatoOperazioni)
    {
    case 0:
        // in questo sottostato devo inizializzare la barra di scorrimento per avere un'idea di quando deve scorrere
        numDevices=0;
        i=0;
        while (strutturaDispositivo[i]!=nullptr)
        {
            numDevices++;
            i++;
        }
        incrementoBarra=100.0/(float)(numDevices*2);    // ogni operazione di program prevede 3 fasi
        valoreBarra=0;
        ui->progressBar->setValue((int)valoreBarra);
        sottoStatoOperazioni=1;
        timeoutProcesso=0;
        dispositivoAttivo=0;
        break;
    case 1:
        // per adesso faccio finta che ci siano i dispositivi da cancellare
        // qui costruisco il file JSON e lancio il file batch
        // per prima cosa costruisco il file json - il processo python si aspetta di trovare il file json nella stessa
        // directory in cui si trova lui
        nomeFileJson=QApplication::applicationDirPath();
        nomeFileJson+="/fileOperazioni.json";
        i=0;
        while (strutturaDispositivo[i]!=nullptr)
        {
            // costruisco gli oggetti JSON
            oggettoInserito["operazione"]="verify";
            oggettoInserito["indSeriale"]=strutturaDispositivo[i]->IDdispositivo;
            oggettoInserito["nomeFile"]=strutturaDispositivo[i]->nomeFile;
            Stringa="operazione"+ Stringa1.sprintf("%d",i+1);
            i++;
            oggettoRoot.insert(Stringa,oggettoInserito);
        }
        numeroOggettiJSON=i;

        // in oggetto root ho tutte le operazioni di erase da fare
        documentoJson= new QJsonDocument(oggettoRoot);

        stringaJson=documentoJson->toJson();
        fileJson=new QFile(nomeFileJson);
        if (!fileJson->open(QIODevice::WriteOnly))
        {
            // non sono riuscito ad aprire il file, cacca!!
            QMessageBox msgbox;
            msgbox.setText("Non sono riuscito ad aprire il file JSON!!!!");
            msgbox.setIcon(QMessageBox::Critical);
            msgbox.exec();
            statoOperazioni=STATO_IDLE;
            sottoStatoOperazioni=0;
            break;
        }

        fileJson->write(stringaJson.toLocal8Bit());
        fileJson->close();

        // ho creato il file JSON adesso devo lanciare il processo batch
        pathApp+="/"+NomeFile;
        arguments << "/c" << pathApp;
        pathApp="cmd.exe";
        processoBatch->setWorkingDirectory(QCoreApplication::applicationDirPath());
        processoBatch->start(pathApp,arguments,QProcess::ReadWrite);
        sottoStatoOperazioni=2;
        batchFinito=false;
        break;
    case 2:
        // qui aspetto che il processo batch mi dica di avere finito
        if (batchFinito)
        {
            Stringa=ui->txtStato->toPlainText();
            Stringa1.sprintf("Fase di verify terminata\n");
            Stringa+=Stringa1;
            ui->txtStato->setText(Stringa);

            ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
            statoOperazioni=STATO_IDLE;

            // devo cancellare tutti gli oggetti
            for (i=1;i<=numeroOggettiJSON;i++)
            {
                Stringa="operazione"+Stringa1.sprintf("%d",i);
                oggettoRoot.remove(Stringa);
            }
            break;
        }

        break;

    default:
        sottoStatoOperazioni=0;
        break;
    }
}


void MainWindow::gestisciStatoProgram()
{
    int i;
    int numDevices;
    QString Stringa;
    QString Stringa1;
    QString nomeFileJson;
    QString pathApp = QCoreApplication::applicationDirPath();
    QString NomeFile = "leggiFilebatch.bat";
    QStringList arguments;
    QFile *fileJson;
    QString stringaJson;
    QJsonDocument *documentoJson;

    // in questa funione devo gestire la sequenza di operazioni per lo stato di erase - devo creare il file json con i
    // diversi slave su cui effettuare l'erase
    switch (sottoStatoOperazioni)
    {
    case 0:
        // in questo sottostato devo inizializzare la barra di scorrimento per avere un'idea di quando deve scorrere
        numDevices=0;
        i=0;
        while (strutturaDispositivo[i]!=nullptr)
        {
            numDevices++;
            i++;
        }
        incrementoBarra=100.0/(float)(numDevices*3);    // ogni operazione di program prevede 3 fasi
        valoreBarra=0;
        ui->progressBar->setValue((int)valoreBarra);
        sottoStatoOperazioni=1;
        timeoutProcesso=0;
        dispositivoAttivo=0;
        break;
    case 1:
        // per adesso faccio finta che ci siano i dispositivi da cancellare
/*        Stringa=ui->txtStato->toPlainText();
        if (strutturaDispositivo[dispositivoAttivo]->IDdispositivo==0xFF)
        {
            // eraso la MAB
            Stringa1="Cancellazione MAB attivata\n";
        }
        else if ((strutturaDispositivo[dispositivoAttivo]->IDdispositivo>=1) && (strutturaDispositivo[dispositivoAttivo]->IDdispositivo<=8))
        {
            // eraso una base
            Stringa1.sprintf("Cancellazione base %d attivata\n",strutturaDispositivo[dispositivoAttivo]->IDdispositivo);
        }
        else if ((strutturaDispositivo[dispositivoAttivo]->IDdispositivo>=9) && (strutturaDispositivo[dispositivoAttivo]->IDdispositivo<=0x20))
        {
            // è un colore
            Stringa1.sprintf("Cancellazione colore %d attivata\n",strutturaDispositivo[dispositivoAttivo]->IDdispositivo-8);
        }
        else if ((strutturaDispositivo[dispositivoAttivo]->IDdispositivo>=0x21) && (strutturaDispositivo[dispositivoAttivo]->IDdispositivo<=0x22))
        {
            // eraso gli assi X e Y
            Stringa1.sprintf("Cancellazione asse %d attivata\n",strutturaDispositivo[dispositivoAttivo]->IDdispositivo-0x20);
        }
        else if ((strutturaDispositivo[dispositivoAttivo]->IDdispositivo>=0x23) && (strutturaDispositivo[dispositivoAttivo]->IDdispositivo<=0x26))
        {
            // eraso i container
            Stringa1.sprintf("Cancellazione container %d attivata\n",strutturaDispositivo[dispositivoAttivo]->IDdispositivo-0x22);
        }
        else if ((strutturaDispositivo[dispositivoAttivo]->IDdispositivo>=0x27) && (strutturaDispositivo[dispositivoAttivo]->IDdispositivo<=0x28))
        {
            // eraso i container
            Stringa1.sprintf("Cancellazione cover %d attivata\n",strutturaDispositivo[dispositivoAttivo]->IDdispositivo-0x26);
        }
        else if (strutturaDispositivo[dispositivoAttivo]->IDdispositivo==0x29)
        {
            // eraso i container
            Stringa1.sprintf("Cancellazione autocap attivata\n");
        }
        else if (strutturaDispositivo[dispositivoAttivo]->IDdispositivo==0x2A)
        {
            // eraso i container
            Stringa1.sprintf("Cancellazione CAN lifter attivata\n");
        }
        else if (strutturaDispositivo[dispositivoAttivo]->IDdispositivo==0x2B)
        {
            // eraso i container
            Stringa1.sprintf("Cancellazione humidifier attivata\n");
        }

        Stringa+=Stringa1;
        ui->txtStato->setText(Stringa);

        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
        sottoStatoOperazioni=2;
*/
        // qui costruisco il file JSON e lancio il file batch
        // per prima cosa costruisco il file json - il processo python si aspetta di trovare il file json nella stessa
        // directory in cui si trova lui
        nomeFileJson=QApplication::applicationDirPath();
        nomeFileJson+="/fileOperazioni.json";
        i=0;
        while (strutturaDispositivo[i]!=nullptr)
        {
            // costruisco gli oggetti JSON
            oggettoInserito["operazione"]="program";
            oggettoInserito["indSeriale"]=strutturaDispositivo[i]->IDdispositivo;
            oggettoInserito["nomeFile"]=strutturaDispositivo[i]->nomeFile;
            Stringa="operazione"+ Stringa1.sprintf("%d",i+1);
            i++;
            oggettoRoot.insert(Stringa,oggettoInserito);
            nomeOperazione=Stringa;
        }
        numeroOggettiJSON=i;

        // in oggetto root ho tutte le operazioni di erase da fare
        documentoJson= new QJsonDocument(oggettoRoot);

        stringaJson=documentoJson->toJson();
        fileJson=new QFile(nomeFileJson);
        if (!fileJson->open(QIODevice::WriteOnly))
        {
            // non sono riuscito ad aprire il file, cacca!!
            QMessageBox msgbox;
            msgbox.setText("Non sono riuscito ad aprire il file JSON!!!!");
            msgbox.setIcon(QMessageBox::Critical);
            msgbox.exec();
            statoOperazioni=STATO_IDLE;
            sottoStatoOperazioni=0;
            break;
        }

        fileJson->write(stringaJson.toLocal8Bit());
        fileJson->close();

        // ho creato il file JSON adesso devo lanciare il processo batch
        pathApp+="/"+NomeFile;
        arguments << "/c" << pathApp;
        pathApp="cmd.exe";
        processoBatch->setWorkingDirectory(QCoreApplication::applicationDirPath());
        processoBatch->start(pathApp,arguments,QProcess::ReadWrite);
        sottoStatoOperazioni=2;
        batchFinito=false;
        break;
    case 2:
        // qui aspetto che il processo batch mi dica di avere finito
        if (batchFinito)
        {
            // il batch è finito chiudo la procedura di erase
            Stringa=ui->txtStato->toPlainText();
            Stringa1.sprintf("Fase di programmazione terminata\n");
            Stringa+=Stringa1;
            ui->txtStato->setText(Stringa);

            ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
            statoOperazioni=STATO_IDLE;
            // devo cancellare tutti gli oggetti
            for (i=1;i<=numeroOggettiJSON;i++)
            {
                Stringa="operazione"+Stringa1.sprintf("%d",i);
                oggettoRoot.remove(Stringa);
            }
            break;
        }

        break;

    default:
        sottoStatoOperazioni=0;
        break;
    }

}

void MainWindow::gestisciStatoErase()
{
    int i;
    int numDevices;
    QString Stringa;
    QString Stringa1;
    QString nomeFileJson;
    QString pathApp = QCoreApplication::applicationDirPath();
    QString NomeFile = "leggiFilebatch.bat";
    QStringList arguments;
    QFile *fileJson;
    QString stringaJson;
    QJsonDocument *documentoJson;

    // in questa funione devo gestire la sequenza di operazioni per lo stato di erase - devo creare il file json con i
    // diversi slave su cui effettuare l'erase
    switch (sottoStatoOperazioni)
    {
    case 0:
        // in questo sottostato devo inizializzare la barra di scorrimento per avere un'idea di quando deve scorrere
        numDevices=0;
        i=0;
        while (strutturaDispositivo[i]!=nullptr)
        {
            numDevices++;
            i++;
        }
        incrementoBarra=100.0/(float)numDevices;
        valoreBarra=0;
        ui->progressBar->setValue((int)valoreBarra);
        sottoStatoOperazioni=1;
        timeoutProcesso=0;
        dispositivoAttivo=0;
        break;
    case 1:
        // per adesso faccio finta che ci siano i dispositivi da cancellare
/*        Stringa=ui->txtStato->toPlainText();
        if (strutturaDispositivo[dispositivoAttivo]->IDdispositivo==0xFF)
        {
            // eraso la MAB
            Stringa1="Cancellazione MAB attivata\n";
        }
        else if ((strutturaDispositivo[dispositivoAttivo]->IDdispositivo>=1) && (strutturaDispositivo[dispositivoAttivo]->IDdispositivo<=8))
        {
            // eraso una base
            Stringa1.sprintf("Cancellazione base %d attivata\n",strutturaDispositivo[dispositivoAttivo]->IDdispositivo);
        }
        else if ((strutturaDispositivo[dispositivoAttivo]->IDdispositivo>=9) && (strutturaDispositivo[dispositivoAttivo]->IDdispositivo<=0x20))
        {
            // è un colore
            Stringa1.sprintf("Cancellazione colore %d attivata\n",strutturaDispositivo[dispositivoAttivo]->IDdispositivo-8);
        }
        else if ((strutturaDispositivo[dispositivoAttivo]->IDdispositivo>=0x21) && (strutturaDispositivo[dispositivoAttivo]->IDdispositivo<=0x22))
        {
            // eraso gli assi X e Y
            Stringa1.sprintf("Cancellazione asse %d attivata\n",strutturaDispositivo[dispositivoAttivo]->IDdispositivo-0x20);
        }
        else if ((strutturaDispositivo[dispositivoAttivo]->IDdispositivo>=0x23) && (strutturaDispositivo[dispositivoAttivo]->IDdispositivo<=0x26))
        {
            // eraso i container
            Stringa1.sprintf("Cancellazione container %d attivata\n",strutturaDispositivo[dispositivoAttivo]->IDdispositivo-0x22);
        }
        else if ((strutturaDispositivo[dispositivoAttivo]->IDdispositivo>=0x27) && (strutturaDispositivo[dispositivoAttivo]->IDdispositivo<=0x28))
        {
            // eraso i container
            Stringa1.sprintf("Cancellazione cover %d attivata\n",strutturaDispositivo[dispositivoAttivo]->IDdispositivo-0x26);
        }
        else if (strutturaDispositivo[dispositivoAttivo]->IDdispositivo==0x29)
        {
            // eraso i container
            Stringa1.sprintf("Cancellazione autocap attivata\n");
        }
        else if (strutturaDispositivo[dispositivoAttivo]->IDdispositivo==0x2A)
        {
            // eraso i container
            Stringa1.sprintf("Cancellazione CAN lifter attivata\n");
        }
        else if (strutturaDispositivo[dispositivoAttivo]->IDdispositivo==0x2B)
        {
            // eraso i container
            Stringa1.sprintf("Cancellazione humidifier attivata\n");
        }

        Stringa+=Stringa1;
        ui->txtStato->setText(Stringa);

        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
        sottoStatoOperazioni=2;
*/
        // qui costruisco il file JSON e lancio il file batch
        // per prima cosa costruisco il file json - il processo python si aspetta di trovare il file json nella stessa
        // directory in cui si trova lui
        nomeFileJson=QApplication::applicationDirPath();
        nomeFileJson+="/fileOperazioni.json";
        i=0;
        while (strutturaDispositivo[i]!=nullptr)
        {
            // costruisco gli oggetti JSON
            oggettoInserito["operazione"]="erase";
            oggettoInserito["indSeriale"]=strutturaDispositivo[i]->IDdispositivo;
            oggettoInserito["nomeFile"]="";
            Stringa="operazione"+ Stringa1.sprintf("%d",i+1);
            i++;
            oggettoRoot.insert(Stringa,oggettoInserito);
            nomeOperazione=Stringa;
        }
        numeroOggettiJSON=i;

        // in oggetto root ho tutte le operazioni di erase da fare
        documentoJson= new QJsonDocument(oggettoRoot);

        stringaJson=documentoJson->toJson();
        fileJson=new QFile(nomeFileJson);
        if (!fileJson->open(QIODevice::WriteOnly))
        {
            // non sono riuscito ad aprire il file, cacca!!
            QMessageBox msgbox;
            msgbox.setText("Non sono riuscito ad aprire il file JSON!!!!");
            msgbox.setIcon(QMessageBox::Critical);
            msgbox.exec();
            statoOperazioni=STATO_IDLE;
            sottoStatoOperazioni=0;
            break;
        }

        fileJson->write(stringaJson.toLocal8Bit());
        fileJson->close();

        // ho creato il file JSON adesso devo lanciare il processo batch
        pathApp+="/"+NomeFile;
        arguments << "/c" << pathApp;
        pathApp="cmd.exe";
        processoBatch->setWorkingDirectory(QCoreApplication::applicationDirPath());
        processoBatch->start(pathApp,arguments,QProcess::ReadWrite);
        sottoStatoOperazioni=2;
        batchFinito=false;
        break;
    case 2:
        // qui aspetto che il processo batch mi dica di avere finito
        if (batchFinito)
        {
            // il batch è finito chiudo la procedura di erase
            Stringa=ui->txtStato->toPlainText();
            Stringa1.sprintf("Fase di erase terminata\n");
            Stringa+=Stringa1;
            ui->txtStato->setText(Stringa);

            ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
            statoOperazioni=STATO_IDLE;
            // devo cancellare tutti gli oggetti
            for (i=1;i<=numeroOggettiJSON;i++)
            {
                Stringa="operazione"+Stringa1.sprintf("%d",i);
                oggettoRoot.remove(Stringa);
            }
            break;
        }

        break;

    default:
        sottoStatoOperazioni=0;
        break;
    }
}

void MainWindow::erroreProcesso(QProcess::ProcessError errore)
{
    QString Stringa;
    Stringa.sprintf("%d",(int)errore);

    // a seconda di dove sono devo resettare il processo
    QMessageBox msgbox;
    msgbox.setText("Errore processo:"+Stringa);
    msgbox.setIcon(QMessageBox::Critical);
    msgbox.exec();

    statoOperazioni=STATO_IDLE;
    sottoStatoOperazioni=0;
}

void MainWindow::partitoProcesso()
{
    timAttesaProcesso->start(100);

}

void MainWindow::timProcessoScaduto()
{
    if (processoBatch->state()==QProcess::NotRunning)
    {
        QMessageBox msgbox;
        msgbox.setText("Il processo batch è terminato!");
        msgbox.setIcon(QMessageBox::Information);
        msgbox.exec();
        timAttesaProcesso->stop();
        batchFinito=true;
    }
}

void MainWindow::illuminaCheckBox(int attuatore)
{
    switch (attuatore)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
        this->chkAttuatori[attuatore-1]->setStyleSheet("color: rgb(85, 170, 255)");
        break;
    case 33:
        ui->chkAsseX->setStyleSheet("color: rgb(85, 170, 255)");
        break;
    case 34:
        ui->chkAsseY->setStyleSheet("color: rgb(85, 170, 255)");
        break;
    case 35:
        ui->chkContainer1->setStyleSheet("color: rgb(85, 170, 255)");
        break;
    case 36:
        ui->chkContainer2->setStyleSheet("color: rgb(85, 170, 255)");
        break;
    case 37:
        ui->chkContainer3->setStyleSheet("color: rgb(85, 170, 255)");
        break;
    case 38:
        ui->chkContainer4->setStyleSheet("color: rgb(85, 170, 255)");
        break;
    case 39:
        ui->chkCover1->setStyleSheet("color: rgb(85, 170, 255)");
        break;
    case 40:
        ui->chkCover2->setStyleSheet("color: rgb(85, 170, 255)");
        break;
    case 41:
        ui->chkAutocap->setStyleSheet("color: rgb(85, 170, 255)");
        break;
    case 42:
        ui->chkCanLifter->setStyleSheet("color: rgb(85, 170, 255)");
        break;
    case 43:
        ui->chkHumidifier->setStyleSheet("color: rgb(85, 170, 255)");
    case 255:
        ui->chkMAB->setStyleSheet("color: rgb(85, 170, 255)");
    }
}
void MainWindow::printOutput()
{
    QByteArray byteArray = processoBatch->readAllStandardOutput();
    QStringList strLines = QString(byteArray).split("\n");
    QString line;
    QString Stringa;
    QString Stringa1;
    uint32_t i;
    QJsonObject oggetto;

    // in questo evento devo andare a vedere cosa viene scritto in modo da poter far avanzare il processo
    switch (statoOperazioni)
    {
    case STATO_IDLE:
        // qui non dovrei proprio arrivarci...

        break;
    case STATO_ERASE:
        switch (sottoStatoOperazioni)
        {
        case 0:
            // anche qui non dovrei passarci
            break;
        case 1:
            // anche qui non dovrei passarci
            break;
        case 2:
            // qui bisogna analizzare le stringhe che arrivano
            for (i=this->indiceStringheStdOut;i<strLines.count();i++)
            {
                line=strLines[i];
                if (line.contains("Process - "))
                {
                    // arrivata comunicazione di processo
                    Stringa=line.mid(10,line.length()-10);

                    if (Stringa.contains("terminata con successo"))
                    {
                        valoreBarra+=incrementoBarra;
                        ui->progressBar->setValue((int)valoreBarra);
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Processo erase per la MAB terminato\n");
                            ui->chkMAB->setStyleSheet("color: rgb(0, 0, 0)");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Processo erase per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " terminato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                        illuminaCheckBox(oggetto["indSeriale"].toInt());
                    }
                    else if (Stringa.contains("iniziata"))
                    {
                        // è la comunicazione di inizio programmazione
                        // qui devo caricare il numero di operazione
                        nomeOperazione=Stringa.left(Stringa.length()-10);
//                        nomeOperazione=nomeOperazione.left(nomeOperazione.length()-1);
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Processo erase per la MAB iniziato\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Processo erase per l'attuatore "+Stringa1.sprintf("%d",(int)oggetto["indSeriale"].toInt())+ " iniziato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

                    }

                }
                else if (line.contains("Error - "))
                {
                    // c'è stato un errore nel processo di erase
                    if (line.contains("bootloader"))
                    {
                        // l'errore è relativo al bootloader
                        ui->txtStato->append("Il bootloader non risponde\n");
                    }
                    else if (line.contains("Dati JSON"))
                    {
                        // c'è un problema con il file JSON
                        ui->txtStato->append("Il file JSON non è valido o non esiste\n");
                    }
                    else if (oggetto["indSeriale"]==255)
                    {
                        // è la MAB
                        ui->txtStato->append("Processo erase per la MAB fallito\n");
                    }
                    else
                    {
                        Stringa1="";
                        ui->txtStato->append("Processo erase per l'attuatore "+Stringa1.sprintf("%d",(int)oggetto["indSeriale"].toInt())+ " iniziato\n");
                    }
                    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                }
            }

//            this->indiceStringheStdOut=i-1;
            break;
        }
        break;
    case STATO_PROGRAM:
        // siamo nello stato di program - vado a vedere se per caso va tutto bene
        switch (sottoStatoOperazioni)
        {
        case 0:
            // anche qui non dovrei passarci
            break;
        case 1:
            // anche qui non dovrei passarci
            break;
        case 2:
            // qui bisogna analizzare le stringhe che arrivano
            for (i=indiceStringheStdOut;i<strLines.count();i++)
            {
                line=strLines[i];
                if (line.contains("Process - "))
                {
                    // arrivata comunicazione di processo
                    Stringa=line.mid(10,line.length()-10);
                    if (Stringa.contains("terminata con successo"))
                    {
                        valoreBarra+=incrementoBarra;
                        ui->progressBar->setValue((int)valoreBarra);
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Processo programmazione per la MAB terminato\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Processo programmazione per l'attuatore "+Stringa1.sprintf("%d",(int)oggetto["indSeriale"].toInt())+ " terminato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                        illuminaCheckBox(oggetto["indSeriale"].toInt());

                    }
                    else if (Stringa.contains("erase attivata"))
                    {
                        // è la comunicazione di inizio erase
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Processo erase per la MAB iniziato\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Processo erase per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

                    }
                    else if (Stringa.contains("program attivata"))
                    {
                        // iniziato il processo di programmazione
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Processo programmazione per la MAB iniziato\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Processo programmazione per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                        valoreBarra+=incrementoBarra;
                        ui->progressBar->setValue((int)valoreBarra);
                    }
                    else if (Stringa.contains("verify attivata"))
                    {
                        // il processo di programmazione è terminato ed inizia il verify
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Processo verifica per la MAB iniziato\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Processo verifica per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                        valoreBarra+=incrementoBarra;
                        ui->progressBar->setValue((int)valoreBarra);
                    }
                    else if (Stringa.contains("iniziata"))
                    {
                        // mi dice quale operazione è cominciata
                        nomeOperazione=Stringa.left(Stringa.length()-10);
//                        nomeOperazione=nomeOperazione.left(nomeOperazione.length()-1);
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Programmazione della MAB iniziata\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Programmazione dell'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziata\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

                    }
                }
                else if (line.contains("Error - "))
                {
                    // c'è stato un errore nel processo di erase
                    if (line.contains("bootloader"))
                    {
                        // l'errore è relativo al bootloader
                        ui->txtStato->append("Il bootloader non risponde\n");
                    }
                    else if (line.contains("Dati JSON"))
                    {
                        // c'è un problema con il file JSON
                        ui->txtStato->append("Il file JSON non è valido o non esiste\n");
                    }
                    else if (oggetto["indSeriale"]==255)
                    {
                        // è la MAB
                        ui->txtStato->append("Processo programmazione per la MAB fallito\n");
                    }
                    else
                    {
                        Stringa1="";
                        ui->txtStato->append("Processo programmazione per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziato\n");
                    }
                    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                }
            }
//            indiceStringheStdOut=i;

            break;
        }
        break;

    case STATO_QUERY:
        switch (sottoStatoOperazioni)
        {
        case 0:
            // anche qui non dovrei passarci
            break;
        case 1:
            // anche qui non dovrei passarci
            break;
        case 2:
            // qui bisogna analizzare le stringhe che arrivano
            for (i=indiceStringheStdOut;i<strLines.count();i++)
            {
                line=strLines[i];
                if (line.contains("Process - "))
                {
                    // arrivata comunicazione di processo
                    Stringa=line.mid(10,line.length()-10);
                    oggetto=oggettoRoot[nomeOperazione].toObject();
                    if (Stringa.contains("terminata con successo"))
                    {
                        valoreBarra+=incrementoBarra;
                        ui->progressBar->setValue((int)valoreBarra);
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Processo query per la MAB terminato\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Processo query per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " terminato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                        illuminaCheckBox(oggetto["indSeriale"].toInt());

                    }
                    else if (Stringa.contains("query attivata"))
                    {
                        // è la comunicazione di inizio query
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Processo query per la MAB iniziato\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Processo query per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

                    }
                    else if (Stringa.contains("iniziata"))
                    {
                        // mi sta dicendo che l'operazione è iniziata
                        nomeOperazione=Stringa.left(Stringa.length()-10);
//                        nomeOperazione=nomeOperazione.left(nomeOperazione.length()-1);
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Query della MAB iniziata\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Query dell'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziata\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

                    }
                }
                else if (line.contains("Error - "))
                {
                    // c'è stato un errore nel processo di erase
                    if (line.contains("bootloader"))
                    {
                        // l'errore è relativo al bootloader
                        ui->txtStato->append("Il bootloader non risponde\n");
                    }
                    else if (line.contains("Dati JSON"))
                    {
                        // c'è un problema con il file JSON
                        ui->txtStato->append("Il file JSON non è valido o non esiste\n");
                    }
                    else if (oggetto["indSeriale"]==255)
                    {
                        // è la MAB
                        ui->txtStato->append("Processo query per la MAB fallito\n");
                    }
                    else
                    {
                        Stringa1="";
                        ui->txtStato->append("Processo query per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziato\n");
                    }
                    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                }
            }
//            indiceStringheStdOut=i;

            break;
        }

        break;
    case STATO_READ_DEVICES:
        // andiamo a leggere i file dei vari dispositivi
        switch (sottoStatoOperazioni)
        {
        case 0:
            // anche qui non dovrei passarci
            break;
        case 1:
            // anche qui non dovrei passarci
            break;
        case 2:
            // qui bisogna analizzare le stringhe che arrivano
            for (i=indiceStringheStdOut;i<strLines.count();i++)
            {
                line=strLines[i];
                if (line.contains("Process - "))
                {
                    // arrivata comunicazione di processo
                    Stringa=line.mid(10,line.length()-10);
                    if (Stringa.contains("terminata con successo"))
                    {
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        valoreBarra+=incrementoBarra;
                        ui->progressBar->setValue((int)valoreBarra);
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Processo lettura per la MAB terminato\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Processo lettura per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " terminato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                        illuminaCheckBox(oggetto["indSeriale"].toInt());

                    }
                    else if (Stringa.contains("lettura memoria attivata"))
                    {
                        // è la comunicazione di inizio lettura
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Processo lettura per la MAB iniziato\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Processo lettura per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

                    }
                    else if (Stringa.contains("iniziata"))
                    {
                        // mi dice quale operazione è partita
                        nomeOperazione=Stringa.left(Stringa.length()-10);
//                        nomeOperazione=nomeOperazione.left(nomeOperazione.length()-1);
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Lettura della MAB iniziata\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Lettura dell'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziata\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                    }
                }
                else if (line.contains("Error - "))
                {
                    // c'è stato un errore nel processo di erase
                    if (line.contains("bootloader"))
                    {
                        // l'errore è relativo al bootloader
                        ui->txtStato->append("Il bootloader non risponde\n");
                    }
                    else if (line.contains("Dati JSON"))
                    {
                        // c'è un problema con il file JSON
                        ui->txtStato->append("Il file JSON non è valido o non esiste\n");
                    }
                    else if (oggetto["indSeriale"]==255)
                    {
                        // è la MAB
                        ui->txtStato->append("Processo lettura per la MAB fallito\n");
                    }
                    else
                    {
                        Stringa1="";
                        ui->txtStato->append("Processo lettura per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziato\n");
                    }
                    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                }
            }
//            indiceStringheStdOut=i;

            break;
        }

        break;
    case STATO_RESET:
        switch (sottoStatoOperazioni)
        {
        case 0:
            // anche qui non dovrei passarci
            break;
        case 1:
            // anche qui non dovrei passarci
            break;
        case 2:
            // qui bisogna analizzare le stringhe che arrivano
            for (i=indiceStringheStdOut;i<strLines.count();i++)
            {
                line=strLines[i];
                if (line.contains("Process - "))
                {
                    // arrivata comunicazione di processo
                    Stringa=line.mid(10,line.length()-10);
                    if (Stringa.contains("terminata con successo"))
                    {
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        valoreBarra+=incrementoBarra;
                        ui->progressBar->setValue((int)valoreBarra);
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Processo reset per la MAB terminato\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Processo reset per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " terminato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                        illuminaCheckBox(oggetto["indSeriale"].toInt());

                    }
                    else if (Stringa.contains("reset attivata"))
                    {
                        // è la comunicazione di inizio lettura
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Processo reset per la MAB iniziato\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Processo reset per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

                    }
                    else if (Stringa.contains("iniziata"))
                    {
                        // mi dice il numero dell'operazione
                        nomeOperazione=Stringa.left(Stringa.length()-10);
//                        nomeOperazione=nomeOperazione.left(nomeOperazione.length()-1);
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Reset della MAB iniziata\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Reset dell'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziata\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                    }
                }
                else if (line.contains("Error - "))
                {
                    // c'è stato un errore nel processo di erase
                    if (line.contains("bootloader"))
                    {
                        // l'errore è relativo al bootloader
                        ui->txtStato->append("Il bootloader non risponde\n");
                    }
                    else if (line.contains("Dati JSON"))
                    {
                        // c'è un problema con il file JSON
                        ui->txtStato->append("Il file JSON non è valido o non esiste\n");
                    }
                    else if (oggetto["indSeriale"]==255)
                    {
                        // è la MAB
                        ui->txtStato->append("Processo reset per la MAB fallito\n");
                    }
                    else
                    {
                        Stringa1="";
                        ui->txtStato->append("Processo reset per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziato\n");
                    }
                    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                }
            }
//            indiceStringheStdOut=i;

            break;
        }

        break;
    case STATO_VERIFY:
        switch (sottoStatoOperazioni)
        {
        case 0:
            // anche qui non dovrei passarci
            break;
        case 1:
            // anche qui non dovrei passarci
            break;
        case 2:
            // qui bisogna analizzare le stringhe che arrivano
            for (i=indiceStringheStdOut;i<strLines.count();i++)
            {
                line=strLines[i];
                if (line.contains("Process - "))
                {
                    // arrivata comunicazione di processo
                    Stringa=line.mid(10,line.length()-10);
                    if (Stringa.contains("terminata con successo"))
                    {
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        valoreBarra+=incrementoBarra;
                        ui->progressBar->setValue((int)valoreBarra);
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Processo di verifica per la MAB terminato\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Processo di verifica per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " terminato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                        illuminaCheckBox(oggetto["indSeriale"].toInt());

                    }
                    else if (Stringa.contains("file hex attivata"))
                    {
                        // è la comunicazione di inizio lettura
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Caricamento file hex per la MAB iniziato\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Caricamento file hex per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

                    }
                    else if (Stringa.contains("verifica attivata"))
                    {
                        // è la comunicazione di inizio lettura
                        valoreBarra+=incrementoBarra;
                        ui->progressBar->setValue((int)valoreBarra);
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Processo di verifica per la MAB iniziato\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Processo di verifica per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziato\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                    }
                    else if (Stringa.contains("iniziata"))
                    {
                        // mi dice il numero dell'operazione in corso
                        nomeOperazione=Stringa.left(Stringa.length()-10);
//                        nomeOperazione=nomeOperazione.left(nomeOperazione.length()-1);
                        oggetto=oggettoRoot[nomeOperazione].toObject();
                        if (oggetto["indSeriale"]==255)
                        {
                            // è la MAB
                            ui->txtStato->append("Verifica della MAB iniziata\n");
                        }
                        else
                        {
                            Stringa1="";
                            ui->txtStato->append("Verifica dell'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziata\n");
                        }
                        ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

                    }
                }
                else if (line.contains("Error - "))
                {
                    // c'è stato un errore nel processo di erase
                    if (line.contains("bootloader"))
                    {
                        // l'errore è relativo al bootloader
                        ui->txtStato->append("Il bootloader non risponde\n");
                    }
                    else if (line.contains("Dati JSON"))
                    {
                        // c'è un problema con il file JSON
                        ui->txtStato->append("Il file JSON non è valido o non esiste\n");
                    }
                    else if (oggetto["indSeriale"]==255)
                    {
                        // è la MAB
                        ui->txtStato->append("Processo di verifica per la MAB fallito\n");
                    }
                    else
                    {
                        Stringa1="";
                        ui->txtStato->append("Processo di verifica per l'attuatore "+Stringa1.sprintf("%d",oggetto["indSeriale"].toInt())+ " iniziato\n");
                    }
                    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
                }
            }

//            indiceStringheStdOut=i;

            break;

        }

        break;
    }

}

void MainWindow::printError()
{
//    ui->txtPrintError->append("Got to printError()");

    QByteArray byteArray = processoBatch->readAllStandardError();
    QStringList strLines = QString(byteArray).split("\n");
/*
    foreach (QString line, strLines){
        ui->txtPrintError->append(line);
    }
*/

}

void MainWindow::on_btnQuery_clicked()
{
    if (!dispositivoSel())
    {
        // non è stato selezionato nessun dispositivo - non faccio nulla
        QMessageBox msgBox;

        msgBox.setText("Non è stato selezionato nessun dispositivo!!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        return;
    }

    // ci sono dei dispositivi selezionati procedo con il processo di erase
    ui->btnDumpMemory->setEnabled(false);
    ui->btnErase->setEnabled(false);
    ui->btnExportHex->setEnabled(false);
    ui->btnProgramVerify->setEnabled(false);
    ui->btnErase->setEnabled(false);
    ui->btnReadDevices->setEnabled(false);
    ui->btnResetDevice->setEnabled(false);
    ui->btnVerify->setEnabled(false);

    statoOperazioni=STATO_QUERY;
    sottoStatoOperazioni=0;
    spegniCheckBox();

}

void MainWindow::on_btnReadDevices_clicked()
{
    if (!dispositivoSel())
    {
        // non è stato selezionato nessun dispositivo - non faccio nulla
        QMessageBox msgBox;

        msgBox.setText("Non è stato selezionato nessun dispositivo!!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        return;
    }

    // ci sono dei dispositivi selezionati procedo con il processo di erase
    ui->btnDumpMemory->setEnabled(false);
    ui->btnErase->setEnabled(false);
    ui->btnExportHex->setEnabled(false);
    ui->btnProgramVerify->setEnabled(false);
    ui->btnQuery->setEnabled(false);
    ui->btnReadDevices->setEnabled(false);
    ui->btnResetDevice->setEnabled(false);
    ui->btnVerify->setEnabled(false);

    statoOperazioni=STATO_READ_DEVICES;
    sottoStatoOperazioni=0;
    spegniCheckBox();

}

void MainWindow::on_btnExportHex_clicked()
{
    if (!dispositivoSel())
    {
        // non è stato selezionato nessun dispositivo - non faccio nulla
        QMessageBox msgBox;

        msgBox.setText("Non è stato selezionato nessun dispositivo!!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        return;
    }

    // ci sono dei dispositivi selezionati procedo con il processo di erase
    ui->btnDumpMemory->setEnabled(false);
    ui->btnErase->setEnabled(false);
    ui->btnExportHex->setEnabled(false);
    ui->btnProgramVerify->setEnabled(false);
    ui->btnQuery->setEnabled(false);
    ui->btnReadDevices->setEnabled(false);
    ui->btnResetDevice->setEnabled(false);
    ui->btnVerify->setEnabled(false);

    statoOperazioni=STATO_EXPORT_HEX;
    sottoStatoOperazioni=0;
    spegniCheckBox();

}

void MainWindow::on_btnDumpMemory_clicked()
{

}

void MainWindow::on_btnProgramVerify_clicked()
{
    if (!dispositivoSel())
    {
        // non è stato selezionato nessun dispositivo - non faccio nulla
        QMessageBox msgBox;

        msgBox.setText("Non è stato selezionato nessun dispositivo!!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        return;
    }

    // ci sono dei dispositivi selezionati procedo con il processo di erase
    ui->btnDumpMemory->setEnabled(false);
    ui->btnErase->setEnabled(false);
    ui->btnExportHex->setEnabled(false);
    ui->btnProgramVerify->setEnabled(false);
    ui->btnQuery->setEnabled(false);
    ui->btnReadDevices->setEnabled(false);
    ui->btnResetDevice->setEnabled(false);
    ui->btnVerify->setEnabled(false);

    statoOperazioni=STATO_PROGRAM;
    sottoStatoOperazioni=0;
    spegniCheckBox();

}

void MainWindow::on_btnVerify_clicked()
{
    if (!dispositivoSel())
    {
        // non è stato selezionato nessun dispositivo - non faccio nulla
        QMessageBox msgBox;

        msgBox.setText("Non è stato selezionato nessun dispositivo!!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        return;
    }

    // ci sono dei dispositivi selezionati procedo con il processo di erase
    ui->btnDumpMemory->setEnabled(false);
    ui->btnErase->setEnabled(false);
    ui->btnExportHex->setEnabled(false);
    ui->btnProgramVerify->setEnabled(false);
    ui->btnQuery->setEnabled(false);
    ui->btnReadDevices->setEnabled(false);
    ui->btnResetDevice->setEnabled(false);
    ui->btnVerify->setEnabled(false);

    statoOperazioni=STATO_VERIFY;
    sottoStatoOperazioni=0;
    spegniCheckBox();

}

void MainWindow::on_btnResetDevice_clicked()
{
    if (!dispositivoSel())
    {
        // non è stato selezionato nessun dispositivo - non faccio nulla
        QMessageBox msgBox;

        msgBox.setText("Non è stato selezionato nessun dispositivo!!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        return;
    }

    // ci sono dei dispositivi selezionati procedo con il processo di erase
    ui->btnDumpMemory->setEnabled(false);
    ui->btnErase->setEnabled(false);
    ui->btnExportHex->setEnabled(false);
    ui->btnProgramVerify->setEnabled(false);
    ui->btnQuery->setEnabled(false);
    ui->btnReadDevices->setEnabled(false);
    ui->btnResetDevice->setEnabled(false);
    ui->btnVerify->setEnabled(false);

    statoOperazioni=STATO_RESET;
    sottoStatoOperazioni=0;
    spegniCheckBox();

}

void MainWindow::on_btnClearLog_clicked()
{
    // devo cancellare il log
    ui->txtStato->setText("");
    ui->txtStato->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!visualizzaTasti)
    {
        if (RilevaSequenzaKeyboard==false)
        {
            //if (sequenzaKeyboard)
            QChar ch = sequenzaKeyboard[0];
            if (event->key() == (int)(ch.toLatin1()))
            {
                timKeyboard=new QTimer(this);
                connect(timKeyboard,SIGNAL(timeout()),this,SLOT(timKeyboardScaduto()),Qt::AutoConnection);
                timKeyboard->start(10000);
                numKeyRilevati = 1;
                RilevaSequenzaKeyboard = true;
            }
        }
        else
        {
            QChar ch = sequenzaKeyboard[numKeyRilevati];
            if (event->key() == (int)(ch.toLatin1()))
            {
                numKeyRilevati++;
                if (numKeyRilevati >= sequenzaKeyboard.count())
                {
                    timKeyboard->stop();
                    visualizzaTasti=true;
                }
            }
        }
    }

    /*if(event->key() == Qt::Key_A)
    {
        QMessageBox msgBox;

        msgBox.setText("rilasciato");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }*/
}

void MainWindow::timKeyboardScaduto()
{
    RilevaSequenzaKeyboard = false;
}
