#include "allegrorss.h"
#include "ui_allegrorss.h"

#include <QInputDialog>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QXmlStreamReader>

allegroRSS::allegroRSS(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::allegroRSS)
{
    ui->setupUi(this);
}

allegroRSS::~allegroRSS()
{
    delete ui;
}


void allegroRSS::dodajPrzedmiot() {
//    QString inputTytul = QInputDialog::getText(this,"Podaj szukany przedmiot","Przedmiot",QLineEdit::Normal);
//    QString linkRss = QInputDialog::getText(this,"Kanał RSS","URL",QLineEdit::Normal);
    QString inputTytul = QInputDialog::getText(this,"Podaj szukany przedmiot","Przedmiot",QLineEdit::Normal,"ds");
    QString linkRss = QInputDialog::getText(this,"Kanał RSS","URL",QLineEdit::Normal,"http://allegro.pl/rss.php/search?string=duch+w+sieci&selected_country=1&search_type=1&postcode_enabled=1");


    przedmiot *p = new przedmiot();
    p->przedmiotTytul = inputTytul;
    p->rssUrl = linkRss;

    obserwowanePrzedmioty.append(p);

    odswierzListePrzedmiotow();
    pobierzRss(p->rssUrl);
}


void allegroRSS::odswierzListePrzedmiotow() {
    for (int i = 0; i < obserwowanePrzedmioty.count();i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem();

        item->setText(0,obserwowanePrzedmioty[i]->przedmiotTytul);

        ui->list_przedmoity->addTopLevelItem(item);
    }
}

void allegroRSS::pobierzRss(const QUrl url) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(rssFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(url)));
}

void allegroRSS::rssFinished(QNetworkReply* reply) {
    int itemId = -1;
    for (int i = 0; i < obserwowanePrzedmioty.count(); i++) {
        if (obserwowanePrzedmioty[i]->rssUrl == reply->url()) {
            itemId = i;
            break;
        }
    }

    if (itemId == -1)
        return;

    obserwowanePrzedmioty[itemId]->rssXml = QString(reply->readAll());
//    QMessageBox::information(this,"sd",obserwowanePrzedmioty[itemId]->rssXml,QMessageBox::Ok);
//    QMessageBox::information(this,"sd",obserwowanePrzedmioty[itemId]->rssUrl.toString(),QMessageBox::Ok);

    parseRssXml(itemId);

    reply->deleteLater();
}


void allegroRSS::parseRssXml(const int itemId) {
    QXmlStreamReader xml;
    przedmiot *p = obserwowanePrzedmioty[itemId];

    xml.addData(p->rssXml);

    while (!xml.atEnd()) {
        xml.readNext();

        oferta *o = new oferta();
//        if (xml.name().toString() == "item") {
//            xml.readNext();
//            if (!xml.isStartElement())
//                xml.readNext();

            if (xml.name().toString() == "title") {
                xml.readNext();
                if (xml.isCharacters() && !xml.isWhitespace()) {
                   o->tytul = xml.text().toString();
                    //                QMessageBox::information(this,"asf",xml.text().toString());
                    p->oferty.append(o);
                }

//            }
        }
    }

    for (int q = 0; q< p->oferty.count(); q++) {
       QTreeWidgetItem *itm = new QTreeWidgetItem();

       itm->setText(0,p->oferty[q]->tytul);
       ui->list_oferty->addTopLevelItem(itm);
    }
}

void allegroRSS::on_btn_dodaj_clicked()
{
    dodajPrzedmiot();
}
