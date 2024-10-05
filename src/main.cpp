#include "tinyxml2.h"  // Einfügen der TinyXML2-Bibliothek
#include <hpdf.h>      // Einfügen der libharu-Bibliothek für die PDF-Erstellung
#include <iostream>
#include <string>

// Fehlerbehandlungsfunktion für die PDF-Erstellung
void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data) {
    std::cerr << "PDF Error: " << error_no << ", detail: " << detail_no << std::endl;
}

int main() {
    // *** XML-Erstellung ***

    tinyxml2::XMLDocument doc; // Erstellen eines XML-Dokuments

    // Erstellen der XML-Deklaration
    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);

    // Erstellen des Wurzelelements <document>
    tinyxml2::XMLElement* root = doc.NewElement("document");
    doc.InsertEndChild(root);

    // Hinzufügen eines Elements <h1> mit Textinhalt
    tinyxml2::XMLElement* h1 = doc.NewElement("h1");
    h1->SetText("Überschrift 1");
    root->InsertEndChild(h1);

    // Hinzufügen eines Elements <p> mit verschachtelten Tags
    tinyxml2::XMLElement* p = doc.NewElement("p");
    p->SetText("Ein ");
    root->InsertEndChild(p);

    // Hinzufügen von <strong> innerhalb von <p>
    tinyxml2::XMLElement* strong = doc.NewElement("strong");
    strong->SetText("fetter");
    p->InsertEndChild(strong);

    // Fortsetzen des Textes innerhalb von <p>
    tinyxml2::XMLText* text = doc.NewText(" und ");
    p->InsertEndChild(text);

    // Hinzufügen von <em> innerhalb von <p>
    tinyxml2::XMLElement* em = doc.NewElement("em");
    em->SetText("kursiver");
    p->InsertEndChild(em);

    // Hinzufügen des letzten Teils des Textes
    text = doc.NewText(" Text.");
    p->InsertEndChild(text);

    // Speichern der XML-Datei auf der Festplatte
    doc.SaveFile("example.xml");
    std::cout << "XML-Datei erfolgreich erstellt!" << std::endl;

    // *** PDF-Erstellung basierend auf der XML-Datei ***

    // XML-Datei öffnen und parsen
    if (doc.LoadFile("example.xml") != tinyxml2::XML_SUCCESS) {
        std::cerr << "Fehler beim Laden der XML-Datei!" << std::endl;
        return -1;
    }

    tinyxml2::XMLElement* rootElement = doc.RootElement();  // Wurzelelement <document>

    // PDF-Dokument initialisieren
    HPDF_Doc pdf = HPDF_New(error_handler, nullptr);
    if (!pdf) {
        std::cerr << "Fehler beim Erstellen des PDF-Dokuments!" << std::endl;
        return -1;
    }

    // Erstellen einer neuen PDF-Seite
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    // Schriftart und Größe festlegen
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", "WinAnsiEncoding");
    HPDF_Page_SetFontAndSize(page, font, 12);

    // Schreiben von Text aus der XML-Datei ins PDF
    float ypos = 800; // Startposition für den Text auf der PDF-Seite

    // Durchlaufen der XML-Elemente
    for (tinyxml2::XMLElement* element = rootElement->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
        const char* tagName = element->Name();
        const char* elementText = element->GetText();

        if (strcmp(tagName, "h1") == 0) {
            // Überschrift 1
            HPDF_Page_BeginText(page);
            HPDF_Page_SetFontAndSize(page, font, 24);  // Größere Schriftart für Überschriften
            HPDF_Page_TextOut(page, 50, ypos, elementText);
            HPDF_Page_EndText(page);
            ypos -= 30;  // Platz für die nächste Zeile
        } else if (strcmp(tagName, "p") == 0) {
            // Absatz
            for (tinyxml2::XMLElement* child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
                const char* childTagName = child->Name();
                const char* childText = child->GetText();

                if (strcmp(childTagName, "strong") == 0) {
                    // Fettdruck
                    HPDF_Page_BeginText(page);
                    HPDF_Page_SetFontAndSize(page, font, 12);
                    HPDF_Page_SetTextRenderingMode(page, HPDF_FILL_THEN_STROKE);  // Fettdruck
                    HPDF_Page_TextOut(page, 50, ypos, childText);
                    HPDF_Page_EndText(page);
                    ypos -= 20;
                } else if (strcmp(childTagName, "em") == 0) {
                    // Kursiv
                    HPDF_Page_BeginText(page);
                    HPDF_Page_SetFontAndSize(page, font, 12);
                    HPDF_Page_SetTextRenderingMode(page, HPDF_FILL);  // Kursiv (möglicherweise durch andere Schrift ersetzen)
                    HPDF_Page_TextOut(page, 50, ypos, childText);
                    HPDF_Page_EndText(page);
                    ypos -= 20;
                }
            }
        }
    }

    // PDF-Datei speichern
    HPDF_SaveToFile(pdf, "output.pdf");
    HPDF_Free(pdf);

    std::cout << "PDF-Datei erfolgreich erstellt!" << std::endl;

    return 0;
}
