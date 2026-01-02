#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void ogrenciEkle();
void ogrenciSil();
void ogrencileriGoruntule();
void hesapla();
void minMax();
void gecenKalan();
int otomatikID();

struct Ogrenci {
    int ogrenciID;
    char ogrenciAd[50];
    char ogrenciSoyad[50];
    int ogrenciVize;
    int ogrenciFinal;
    float ogrenciOrtalama;
    char harfNotu[3];
};

char *harfNotuHesapla(float ogrOrt, float sinifOrt){
    float fark = ogrOrt - sinifOrt;

    if(fark >= 15) return "AA";
    else if(fark >= 10) return "BA";
    else if(fark >= 5)  return "BB";
    else if(fark >= 0)  return "CB";
    else if(fark >= -5) return "CC";
    else if(fark >= -10)return "DC";
    else if(fark >= -15)return "DD";
    else return "FF";
}

int main(){
    int secim;

    while(1){
        printf("\n------- OGRENCI SISTEMI -------\n");
        printf("1 - Ogrenci Ekle\n");
        printf("2 - Ogrenci Sil\n");
        printf("3 - Tum Ogrencileri Listele\n");
        printf("4 - Sinif Ortalamasi & Harf Notlari\n");
        printf("5 - Min-Max , Mod, Medyan Hesapla\n");
        printf("6 - Gecen Ve Kalanlari Hesapla\n");
        printf("7 - Cikis\n");
        printf("Seciminizi Yapin : ");

        scanf("%d",&secim);
        getchar();

        switch(secim){
            case 1: ogrenciEkle(); break;
            case 2: ogrenciSil(); break;
            case 3: ogrencileriGoruntule(); break;
            case 4: hesapla(); break;
            case 5: minMax(); break;
            case 6: gecenKalan(); break;
            case 7:
                printf("Programdan cikiliyor...\n");
                exit(0);
            default:
                printf("Hatali Giris!\n");
        }
    }
}


int otomatikID(){
    FILE *dosya = fopen("ogrenciListe.txt","a+");
    int sayac = 0;
    char satir[200];

    if(dosya == NULL) return 1;

    while(fgets(satir, sizeof(satir), dosya))
        sayac++;

    fclose(dosya);
    return sayac + 1;
}

void ogrenciEkle(){
    struct Ogrenci o;

    o.ogrenciID = otomatikID();

    FILE *dosya = fopen("ogrenciListe.txt","a+");
    if(dosya == NULL){
        printf("Dosya Acilamadi!\n");
        return;
    }

    printf("\n--- OGRENCI EKLE ---\n");
    printf("Ogrenci ID: %d\n", o.ogrenciID);

    printf("Ad: ");
    fgets(o.ogrenciAd, sizeof(o.ogrenciAd), stdin);
    o.ogrenciAd[strcspn(o.ogrenciAd,"\n")] = '\0';

    printf("Soyad: ");
    fgets(o.ogrenciSoyad, sizeof(o.ogrenciSoyad), stdin);
    o.ogrenciSoyad[strcspn(o.ogrenciSoyad,"\n")] = '\0';

    printf("Vize: ");
    scanf("%d",&o.ogrenciVize);

    printf("Final: ");
    scanf("%d",&o.ogrenciFinal);
    getchar();

    o.ogrenciOrtalama = o.ogrenciVize * 0.4 + o.ogrenciFinal * 0.6;
    strcpy(o.harfNotu, "--");

    fprintf(dosya,"%d %s %s %d %d %.2f %s\n",
            o.ogrenciID, o.ogrenciAd, o.ogrenciSoyad,
            o.ogrenciVize, o.ogrenciFinal,
            o.ogrenciOrtalama, o.harfNotu);

    fclose(dosya);
    printf("Ogrenci Eklendi!\n");
}

void ogrenciSil(){
    int silinecekID;
    struct Ogrenci o;

    FILE *dosya = fopen("ogrenciListe.txt","a+");
    FILE *temp  = fopen("temp.txt","w");

    if(dosya == NULL || temp == NULL){
        printf("Dosya Acilamadi!\n");
        return;
    }

    printf("Silinecek ID: ");
    scanf("%d",&silinecekID);

    int bulundu = 0;

    rewind(dosya);

    while(fscanf(dosya,"%d %s %s %d %d %f %s",
                 &o.ogrenciID, o.ogrenciAd, o.ogrenciSoyad,
                 &o.ogrenciVize, &o.ogrenciFinal,
                 &o.ogrenciOrtalama, o.harfNotu) != EOF){

        if(o.ogrenciID != silinecekID){
            fprintf(temp,"%d %s %s %d %d %.2f %s\n",
                    o.ogrenciID, o.ogrenciAd, o.ogrenciSoyad,
                    o.ogrenciVize, o.ogrenciFinal,
                    o.ogrenciOrtalama, o.harfNotu);
        } else {
            bulundu = 1;
        }
    }

    fclose(dosya);
    fclose(temp);

    remove("ogrenciListe.txt");
    rename("temp.txt","ogrenciListe.txt");

    if(bulundu) printf("Ogrenci Silindi!\n");
    else printf("Ogrenci Bulunamadi!\n");
}

void ogrencileriGoruntule(){
    struct Ogrenci o;

    FILE *dosya = fopen("ogrenciListe.txt","a+");
    if(dosya == NULL){
        printf("Dosya Acilamadi!\n");
        return;
    }

    rewind(dosya);

    printf("\nID  Ad        Soyad      Vize Final Ort   Harf\n");
    printf("------------------------------------------------\n");

    while(fscanf(dosya,"%d %s %s %d %d %f %s",
                 &o.ogrenciID, o.ogrenciAd, o.ogrenciSoyad,
                 &o.ogrenciVize, &o.ogrenciFinal,
                 &o.ogrenciOrtalama, o.harfNotu) != EOF){

        printf("%-3d %-10s %-10s %-4d %-5d %-5.2f %s\n",
               o.ogrenciID, o.ogrenciAd, o.ogrenciSoyad,
               o.ogrenciVize, o.ogrenciFinal,
               o.ogrenciOrtalama, o.harfNotu);
    }

    fclose(dosya);
}

void hesapla(){
    struct Ogrenci o;
    float toplam = 0, sinifOrt;
    int sayac = 0;

    FILE *dosya = fopen("ogrenciListe.txt","a+");
    if(dosya == NULL){
        printf("Dosya Acilamadi!\n");
        return;
    }

    rewind(dosya);

    while(fscanf(dosya,"%d %s %s %d %d %f %s",
                 &o.ogrenciID, o.ogrenciAd, o.ogrenciSoyad,
                 &o.ogrenciVize, &o.ogrenciFinal,
                 &o.ogrenciOrtalama, o.harfNotu) != EOF){
        toplam += o.ogrenciOrtalama;
        sayac++;
    }

    fclose(dosya);

    if(sayac == 0){
        printf("Hic ogrenci yok!\n");
        return;
    }

    sinifOrt = toplam / sayac;
    printf("Sinif Ortalamasi: %.2f\n", sinifOrt);

    dosya = fopen("ogrenciListe.txt","r");
    FILE *temp = fopen("temp.txt","w");

    while(fscanf(dosya,"%d %s %s %d %d %f %s",
                 &o.ogrenciID, o.ogrenciAd, o.ogrenciSoyad,
                 &o.ogrenciVize, &o.ogrenciFinal,
                 &o.ogrenciOrtalama, o.harfNotu) != EOF){

        strcpy(o.harfNotu, harfNotuHesapla(o.ogrenciOrtalama, sinifOrt));

        fprintf(temp,"%d %s %s %d %d %.2f %s\n",
                o.ogrenciID, o.ogrenciAd, o.ogrenciSoyad,
                o.ogrenciVize, o.ogrenciFinal,
                o.ogrenciOrtalama, o.harfNotu);
    }

    fclose(dosya);
    fclose(temp);

    remove("ogrenciListe.txt");
    rename("temp.txt","ogrenciListe.txt");

    printf("Harf Notlari Guncellendi!\n");
}

void minMax(){
    struct Ogrenci o, maxOgr, minOgr;

    FILE *dosya = fopen("ogrenciListe.txt","a+");
    if(dosya == NULL){
        printf("Dosya Acilamadi!\n");
        return;
    }

    rewind(dosya);

    if(fscanf(dosya,"%d %s %s %d %d %f %s",
              &o.ogrenciID, o.ogrenciAd, o.ogrenciSoyad,
              &o.ogrenciVize, &o.ogrenciFinal,
              &o.ogrenciOrtalama, o.harfNotu) == EOF){
        printf("Dosya Bos!\n");
        fclose(dosya);
        return;
    }

    maxOgr = minOgr = o;

    while(fscanf(dosya,"%d %s %s %d %d %f %s",
                 &o.ogrenciID, o.ogrenciAd, o.ogrenciSoyad,
                 &o.ogrenciVize, &o.ogrenciFinal,
                 &o.ogrenciOrtalama, o.harfNotu) != EOF){

        if(o.ogrenciOrtalama > maxOgr.ogrenciOrtalama) maxOgr = o;
        if(o.ogrenciOrtalama < minOgr.ogrenciOrtalama) minOgr = o;
    }

    fclose(dosya);

    printf("\n--- EN YUKSEK NOT ---\n");
    printf("%d %s %s %.2f %s\n",
           maxOgr.ogrenciID, maxOgr.ogrenciAd, maxOgr.ogrenciSoyad,
           maxOgr.ogrenciOrtalama, maxOgr.harfNotu);

    printf("\n--- EN DUSUK NOT ---\n");
    printf("%d %s %s %.2f %s\n",
           minOgr.ogrenciID, minOgr.ogrenciAd, minOgr.ogrenciSoyad,
           minOgr.ogrenciOrtalama, minOgr.harfNotu);
}

void gecenKalan(){
    struct Ogrenci o;

    FILE *dosya = fopen("ogrenciListe.txt","a+");
    FILE *gecen = fopen("gecenler.txt","w");
    FILE *but = fopen("butunleme.txt","w");
    FILE *kalan = fopen("kalanlar.txt","w");

    if(dosya == NULL || gecen == NULL || but == NULL || kalan == NULL){
        printf("Dosyalar acilamadi!\n");
        return;
    }

    rewind(dosya);

    while(fscanf(dosya,"%d %s %s %d %d %f %s",
                 &o.ogrenciID, o.ogrenciAd, o.ogrenciSoyad,
                 &o.ogrenciVize, &o.ogrenciFinal,
                 &o.ogrenciOrtalama, o.harfNotu) != EOF){

        if(strcmp(o.harfNotu,"AA") == 0)
            fprintf(gecen,"%d %s %s %.2f %s\n",
                    o.ogrenciID,o.ogrenciAd,o.ogrenciSoyad,
                    o.ogrenciOrtalama,o.harfNotu);
        else if(strcmp(o.harfNotu,"DC")==0 || strcmp(o.harfNotu,"DD")==0)
            fprintf(but,"%d %s %s %.2f %s\n",
                    o.ogrenciID,o.ogrenciAd,o.ogrenciSoyad,
                    o.ogrenciOrtalama,o.harfNotu);
        else if(strcmp(o.harfNotu,"FF")==0)
            fprintf(kalan,"%d %s %s %.2f %s\n",
                    o.ogrenciID,o.ogrenciAd,o.ogrenciSoyad,
                    o.ogrenciOrtalama,o.harfNotu);
    }

    fclose(dosya);
    fclose(gecen);
    fclose(but);
    fclose(kalan);

    printf("Gecen / But / Kalan listeleri olusturuldu!\n");
}