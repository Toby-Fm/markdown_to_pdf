#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <hpdf.h>
#include "cmark.h" // Einfügen der cmark-Bibliothek

// Fehlerbehandlungsfunktion für die PDF-Erstellung
void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void* user_data) {
    std::cerr << "PDF Error: " << error_no << ", detail: " << detail_no << std::endl;
}

int main() {
    // *** Markdown-Datei einlesen ***
    std::string markdown_filename = "test.md";

    std::ifstream markdown_file(markdown_filename);
    if (!markdown_file.is_open()) {
        std::cerr << "Fehler beim Öffnen der Datei " << markdown_filename << "!" << std::endl;
        return -1;
    }

    std::stringstream markdown_buffer;
    markdown_buffer << markdown_file.rdbuf();
    std::string markdown_content = markdown_buffer.str();

    // *** Markdown zu HTML konvertieren ***
    cmark_node* node = cmark_parse_document(markdown_content.c_str(), markdown_content.length(), CMARK_OPT_DEFAULT);
    char* html_content = cmark_render_html(node, CMARK_OPT_DEFAULT);
    cmark_node_free(node);

    // Optional: HTML-Inhalt in eine Datei schreiben (zu Debugging-Zwecken)
    std::ofstream html_file("output.html");
    html_file << html_content;
    html_file.close();

    // *** HTML parsen und PDF erstellen ***
    // PDF-Dokument initialisieren
    HPDF_Doc pdf = HPDF_New(error_handler, nullptr);
    if (!pdf) {
        std::cerr << "Fehler beim Erstellen des PDF-Dokuments!" << std::endl;
        return -1;
    }

    // Verwende die eingebaute Standard-Schriftart Helvetica
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", nullptr);

    // Erstellen einer neuen PDF-Seite
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    // Standard-Schriftart und Größe festlegen
    HPDF_Page_SetFontAndSize(page, font, 12);

    float ypos = 800; // Startposition für den Text auf der PDF-Seite
    const float line_spacing = 20.0f; // Abstand zwischen Textblöcken

    // Einfacher HTML-Parser
    std::istringstream html_stream(html_content);
    std::string line;

    while (std::getline(html_stream, line)) {
        // Entfernen von führenden und nachfolgenden Leerzeichen
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) {
            continue;
        }

        if (line.find("<h1>") != std::string::npos) {
            std::string text = line.substr(4, line.find("</h1>") - 4);
            HPDF_Page_BeginText(page);
            HPDF_Page_SetFontAndSize(page, font, 24);  // Größere Schriftart für Überschrift
            HPDF_Page_TextOut(page, 50, ypos, text.c_str());
            HPDF_Page_EndText(page);
            ypos -= (line_spacing + 30);  // Zusätzlicher Abstand für Überschrift
        } else if (line.find("<p>") != std::string::npos) {
            std::string text = line.substr(3, line.find("</p>") - 3);

            size_t pos = 0;
            while (pos < text.length()) {
                if (text.find("<strong>", pos) == pos) {
                    pos += 8;
                    size_t end = text.find("</strong>", pos);
                    std::string bold_text = text.substr(pos, end - pos);

                    HPDF_Page_BeginText(page);
                    HPDF_Page_SetFontAndSize(page, font, 12);
                    HPDF_Page_TextOut(page, 50, ypos, bold_text.c_str());
                    HPDF_Page_EndText(page);

                    pos = end + 9;
                } else if (text.find("<em>", pos) == pos) {
                    pos += 4;
                    size_t end = text.find("</em>", pos);
                    std::string italic_text = text.substr(pos, end - pos);

                    HPDF_Page_BeginText(page);
                    HPDF_Page_SetFontAndSize(page, font, 12);
                    HPDF_Page_TextOut(page, 50, ypos, italic_text.c_str());
                    HPDF_Page_EndText(page);

                    pos = end + 5;
                } else {
                    size_t next_tag = text.find('<', pos);
                    if (next_tag == std::string::npos) {
                        next_tag = text.length();
                    }
                    std::string normal_text = text.substr(pos, next_tag - pos);

                    HPDF_Page_BeginText(page);
                    HPDF_Page_SetFontAndSize(page, font, 12);
                    HPDF_Page_TextOut(page, 50, ypos, normal_text.c_str());
                    HPDF_Page_EndText(page);

                    pos = next_tag;
                }
            }
            ypos -= line_spacing;  // Abstand zwischen Textblöcken
        }
    }

    // PDF-Datei speichern
    HPDF_SaveToFile(pdf, "output.pdf");
    HPDF_Free(pdf);

    std::cout << "PDF-Datei erfolgreich erstellt!" << std::endl;

    // Speicher freigeben
    free(html_content);

    return 0;
}
