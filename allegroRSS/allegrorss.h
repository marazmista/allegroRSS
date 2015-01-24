#ifndef ALLEGRORSS_H
#define ALLEGRORSS_H

#include <QMainWindow>
#include <QNetworkReply>

namespace Ui {
class allegroRSS;
}

class allegroRSS : public QMainWindow
{
    Q_OBJECT

    struct oferta {
        QString url, picUrl, tytul, typ;
        QList<oferta> ignorowane;
        long id;
        float cena;
    };

    struct przedmiot {
        QString przedmiotTytul, rssXml;
        QUrl rssUrl;
        QList<oferta*> oferty;
    };


public:
    explicit allegroRSS(QWidget *parent = 0);
    ~allegroRSS();

private slots:
    void on_btn_dodaj_clicked();
    void rssFinished(QNetworkReply *reply);


private:
    Ui::allegroRSS *ui;

    QList<przedmiot*> obserwowanePrzedmioty;

    void dodajPrzedmiot();
    void odswierzListePrzedmiotow();
    void pobierzRss(const QUrl url);
    void parseRssXml(const int itemId);
};

#endif // ALLEGRORSS_H
