import subprocess
from reportlab.lib.pagesizes import letter
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer
from reportlab.lib.styles import getSampleStyleSheet
import os

def get_git_log(repo_path):
    """Obtiene el historial de commits del repositorio."""
    os.chdir(repo_path)
    try:
        result = subprocess.run(
            ['git', 'log', '--oneline', '--decorate', '--graph', '--all'],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            encoding='utf-8'
        )
        return result.stdout
    except Exception as e:
        print(f"Error al obtener el log de Git: {e}")
        return ""

def save_log_to_pdf(log_content, output_pdf):
    """Guarda el contenido del log en un archivo PDF."""
    doc = SimpleDocTemplate(output_pdf, pagesize=letter)
    styles = getSampleStyleSheet()
    story = []

    # Título
    title = Paragraph("Historial de Commits del Repositorio", styles['Title'])
    story.append(title)
    story.append(Spacer(1, 12))

    # Contenido del log
    log_para = Paragraph(log_content.replace('\n', '<br/>'), styles['Normal'])
    story.append(log_para)

    doc.build(story)
    print(f"PDF generado: {output_pdf}")

if __name__ == "__main__":
    repo_path = input("Introduce la ruta del repositorio Git: ").strip()
    output_pdf = "github_log.pdf"

    if not os.path.isdir(repo_path):
        print("La ruta especificada no es un directorio válido.")
    else:
        log_content = get_git_log(repo_path)
        if log_content:
            save_log_to_pdf(log_content, output_pdf)
        else:
            print("No se pudo obtener el historial de commits.")
