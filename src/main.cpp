#include "tinyxml2.h"  // Einfügen der TinyXML2-Bibliothek
#include <iostream>

int main() {
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
    return 0;
}
