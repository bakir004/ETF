#ifdef nesta
// ------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TImage *Slika;
	TButton *ButtonOcisti;
	TEdit *EditBrojTacaka;
	TButton *ButtonPresjekKruznica;
	void __fastcall ButtonOcistiClick(TObject *Sender);
	void __fastcall ButtonGenerisiRandomKruzniceClick(TObject *Sender);
	void __fastcall ButtonPresjekKruznicaClick(TObject *Sender);

private:	// User declarations
    vector<Kruznica> kruznice;
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

prozor.cpp:
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "prozor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	Slika->Canvas->Brush->Color=clWhite;
	Slika->Canvas->FillRect(TRect(0,0,Slika->Width,Slika->Height));
	Slika->Canvas->Brush->Color=clBlack;
	Slika->Canvas->FrameRect(TRect(0,0,Slika->Width,Slika->Height));
}

//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonOcistiClick(TObject *Sender)
{
	Slika->Canvas->Brush->Color=clWhite;
	Slika->Canvas->FillRect(TRect(0,0,Slika->Width,Slika->Height));
	Slika->Canvas->Brush->Color=clBlack;
	Slika->Canvas->FrameRect(TRect(0,0,Slika->Width,Slika->Height));

	kruznice.clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonGenerisiRandomKruzniceClick(TObject *Sender)
{
	int brojKruznica = EditBrojTacaka->Text.ToInt();

	for (int i = 0; i < brojKruznica; ++i) {
		int x, y, poluprecnik;
		Tacka centar;
		do {
			x = rand() % Slika->Width;
			y = rand() % Slika->Height;
			poluprecnik = rand() % 100 + 5;
			centar = Tacka(x, y);
		} while (centar.x - poluprecnik < 0 || centar.x + poluprecnik >= Slika->Width ||
				 centar.y - poluprecnik < 0 || centar.y + poluprecnik >= Slika->Height);

		// Kreiranje gornje polukružnice
		Tacka lijevaGornja(centar.x - poluprecnik, centar.y);
		Tacka desnaGornja(centar.x + poluprecnik, centar.y);
		Tacka gornjaGornja(centar.x, centar.y - poluprecnik);
		Tacka donjaGornja(centar.x, centar.y);

		Polukruznica gornja(centar, poluprecnik, lijevaGornja, desnaGornja, gornjaGornja, donjaGornja);

		// Kreiranje donje polukružnice
		Tacka lijevaDonja(centar.x - poluprecnik, centar.y);
        Tacka desnaDonja(centar.x + poluprecnik, centar.y);
        Tacka gornjaDonja(centar.x, centar.y);
		Tacka donjaDonja(centar.x, centar.y + poluprecnik);

		Polukruznica donja(centar, poluprecnik, lijevaDonja, desnaDonja, gornjaDonja, donjaDonja);

		Kruznica nova(gornja, donja);
		nova.Crtaj(Slika, clBlack);
		kruznice.push_back(nova);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonPresjekKruznicaClick(TObject *Sender) {
	// Prioritetna redoslijed događaja za kružnice, sortirano po x-koordinati
	priority_queue<pair<int, pair<Polukruznica*, Polukruznica*>>, vector<pair<int, pair<Polukruznica*, Polukruznica*>>>, PolukruznicePoX> eventi;

	// Balansirano stablo aktivnih polukružnica, sortirano po y-koordinati
    set<Polukruznica*, AktivnePolukruznicePoY> aktivne;

	// Vektor koji će sadržavati tačke presjeka kružnica
    vector<Tacka> presjeci;

	// Inicijalizacija događaja s početnim i završnim tačkama polukružnica
	for (auto& k : kruznice) {
		eventi.push({ k.gornja.lijeva.x, { &k.gornja, &k.donja } });
        eventi.push({ k.gornja.desna.x, { &k.gornja, &k.donja } });
    }

    // Algoritam pomicanja linije
	while (!eventi.empty()) {
        auto event = eventi.top();
		eventi.pop();

		int x_sweep_line = event.first;
        Polukruznica* p1 = event.second.first;
		Polukruznica* p2 = event.second.second;

        // Obrada početnog događaja kružnice p1
        if (event.first == p1->lijeva.x) {
			aktivne.insert(p1);
            aktivne.insert(p2);
        }
		// Obrada završnog događaja kružnice p1
		else if (event.first == p1->desna.x) {
            aktivne.erase(p1);
            aktivne.erase(p2);
        }
		// Obrada presjeka s susjedima
        else {
            // Pronalaženje presjeka između p1 i njegovih susjeda u aktivnim kružnicama
			auto it = aktivne.find(p1);
			if (it != aktivne.begin()) {
				auto prev_it = prev(it);
				obradiPresjekPolukruznica(x_sweep_line, *prev_it, p1, presjeci, eventi);
			}
			auto next_it = next(it);
			if (next_it != aktivne.end()) {
				obradiPresjekPolukruznica(x_sweep_line, p1, *next_it, presjeci, eventi);
			}
		}
    }

	// Iscrtavanje presjeka
	for (auto& presjek : presjeci) {
		presjek.Crtaj(Slika, clRed, 2);
	}
}
//---------------------------------------------------------------------------
