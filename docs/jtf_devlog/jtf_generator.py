#!/usr/bin/env python3
"""
Generador de DevLog JTF - Versión Corregida (Analiza TODOS los commits)
"""

import subprocess
import json
import os
from datetime import datetime
from pathlib import Path

class JTFDevLogGenerator:
    def __init__(self, config_file="jtf_devlog_config.json"):
        self.config = self.load_config(config_file)
        self.repo_path = Path(".")
        
    def load_config(self, config_file):
        """Carga la configuración del proyecto"""
        try:
            with open(config_file, 'r') as f:
                return json.load(f)
        except FileNotFoundError:
            return {
                "project": {"name": "Mi Proyecto"},
                "git": {"max_commits": 50},  # Límite para no saturar
                "output": {
                    "doxygen_file": ".doxygen-devlog",
                    "reports_dir": "docs/jtf_devlog/output"
                }
            }
    
    def run_git_command(self, command):
        """Ejecuta comandos de Git de forma segura"""
        try:
            result = subprocess.run(
                ["git"] + command,
                capture_output=True,
                text=True,
                check=True
            )
            return result.stdout.strip()
        except subprocess.CalledProcessError as e:
            print(f"❌ Error en comando Git: {e}")
            return ""
    
    def get_all_commits(self):
        """Obtiene TODOS los commits de la rama actual"""
        print("🔄 Obteniendo historial completo de commits...")
        
        # Comando para obtener todos los commits con información completa
        cmd = [
            "log", 
            "--all",           # Todas las ramas
            "--pretty=format:%H|%an|%ad|%s", 
            "--date=short",
            "--reverse"        # Orden cronológico (más antiguo primero)
        ]
        
        output = self.run_git_command(cmd)
        
        if not output:
            print("❌ No se pudo obtener el historial de commits")
            return []
        
        commits = []
        lines = output.split('\n')
        
        print(f"📖 Líneas procesadas: {len(lines)}")
        
        for line in lines:
            if '|' in line:
                parts = line.split('|')
                if len(parts) >= 4:
                    commit_data = {
                        'hash': parts[0][:8],  # Short hash
                        'author': parts[1],
                        'date': parts[2],
                        'message': parts[3]
                    }
                    commits.append(commit_data)
                    print(f"   📝 Commit: {commit_data['hash']} - {commit_data['message'][:50]}...")
        
        print(f"✅ Commits encontrados: {len(commits)}")
        return commits
    
    def get_commit_details(self, commit_hash):
        """Obtiene detalles adicionales de un commit específico"""
        try:
            # Archivos modificados en el commit
            files_cmd = ["show", "--name-only", "--pretty=format:", commit_hash]
            files_output = self.run_git_command(files_cmd)
            files = [f.strip() for f in files_output.split('\n') if f.strip()]
            
            # Estadísticas del commit
            stat_cmd = ["show", "--stat", "--pretty=format:", commit_hash]
            stat_output = self.run_git_command(stat_cmd)
            
            return {
                'files': files,
                'statistics': stat_output
            }
        except Exception as e:
            print(f"⚠️ Error obteniendo detalles del commit {commit_hash}: {e}")
            return {'files': [], 'statistics': ''}
    
    def analyze_commit_messages(self, commits):
        """Analiza los mensajes de commit para extraer información útil"""
        print("🔍 Analizando mensajes de commits...")
        
        features = []
        fixes = []
        improvements = []
        others = []
        
        for commit in commits:
            message = commit['message'].lower()
            
            # Detectar tipo de commit
            if any(word in message for word in ['feat', 'feature', 'add', 'nuevo', 'implement', 'crear']):
                commit['type'] = 'feature'
                features.append(commit)
            elif any(word in message for word in ['fix', 'bug', 'correg', 'error', 'solve', 'resolve']):
                commit['type'] = 'fix'
                fixes.append(commit)
            elif any(word in message for word in ['improve', 'mejor', 'optimiz', 'refactor', 'enhance']):
                commit['type'] = 'improvement'
                improvements.append(commit)
            elif any(word in message for word in ['doc', 'document', 'readme', 'comment']):
                commit['type'] = 'documentation'
                others.append(commit)
            else:
                commit['type'] = 'other'
                others.append(commit)
            
            # Obtener detalles adicionales (solo para algunos commits para no saturar)
            if commit['type'] in ['feature', 'fix']:
                details = self.get_commit_details(commit['hash'])
                commit.update(details)
        
        print(f"📊 Análisis completado:")
        print(f"   🎯 Features: {len(features)}")
        print(f"   🐛 Fixes: {len(fixes)}")
        print(f"   ⚡ Mejoras: {len(improvements)}")
        print(f"   📄 Otros: {len(others)}")
        
        return {
            'features': features,
            'fixes': fixes, 
            'improvements': improvements,
            'others': others,
            'total': len(commits)
        }
    
    def generate_doxygen_content(self, analysis):
        """Genera contenido ESPECÍFICO para el devlog del equipo"""
        print("📝 Generando contenido Doxygen...")
        
        doxygen_content = "/**\n"
        doxygen_content += " * @file .doxygen-devlog\n"
        doxygen_content += " * @brief DEVLOG INTERNO - Registro de Desarrollo del Equipo\n"
        doxygen_content += " * @internal\n"
        doxygen_content += " * @team_only\n"
        doxygen_content += " * @generated_automatically\n"
        doxygen_content += " * @date " + datetime.now().strftime("%Y-%m-%d %H:%M") + "\n"
        doxygen_content += " */\n\n"
        
        # Página principal del devlog
        doxygen_content += "/**\n * @page devlog_interno DevLog - Uso Interno del Equipo\n\n"
        doxygen_content += "# 🚀 Registro de Desarrollo Interno\n\n"
        doxygen_content += "**⚠️ ESTA DOCUMENTACIÓN ES SOLO PARA EL EQUIPO DE DESARROLLO**\n\n"
        
        # Estadísticas del equipo
        doxygen_content += "## 📊 Métricas del Equipo\n\n"
        doxygen_content += f"- **Total de commits analizados**: {analysis['total']}\n"
        doxygen_content += f"- **Nuevas funcionalidades**: {len(analysis['features'])}\n"
        doxygen_content += f"- **Correcciones de bugs**: {len(analysis['fixes'])}\n"
        doxygen_content += f"- **Mejoras técnicas**: {len(analysis['improvements'])}\n"
        doxygen_content += f"- **Otros cambios**: {len(analysis['others'])}\n\n"
        
        # Timeline de desarrollo - MOSTRAMOS MÁS COMMITS
        if analysis['features']:
            doxygen_content += "## 🎯 Nuevas Funcionalidades\n\n"
            for commit in analysis['features'][-10:]:  # Últimos 10 features
                doxygen_content += f"- **{commit['date']}** (`{commit['hash']}`): {commit['message']}\n"
                if 'files' in commit and commit['files']:
                    doxygen_content += f"  - Archivos: {', '.join(commit['files'][:3])}"
                    if len(commit['files']) > 3:
                        doxygen_content += f" ... (+{len(commit['files']) - 3} más)"
                    doxygen_content += "\n"
            doxygen_content += "\n"
        
        if analysis['fixes']:
            doxygen_content += "## 🐛 Correcciones Técnicas\n\n"
            for commit in analysis['fixes'][-10:]:  # Últimos 10 fixes
                doxygen_content += f"- **{commit['date']}** (`{commit['hash']}`): {commit['message']}\n"
            doxygen_content += "\n"
        
        if analysis['improvements']:
            doxygen_content += "## ⚡ Mejoras Técnicas\n\n"
            for commit in analysis['improvements'][-5:]:  # Últimas 5 mejoras
                doxygen_content += f"- **{commit['date']}** (`{commit['hash']}`): {commit['message']}\n"
        
        # Resumen de actividad
        doxygen_content += "\n## 📈 Resumen de Actividad\n\n"
        if analysis['total'] > 20:
            doxygen_content += "✅ **Alta actividad** de desarrollo en el proyecto.\n"
        elif analysis['total'] > 5:
            doxygen_content += "⚠️ **Actividad moderada** - considerar aumentar ritmo.\n"
        else:
            doxygen_content += "❌ **Baja actividad** - revisar recursos del equipo.\n"
        
        doxygen_content += "\n*/\n"
        
        return doxygen_content
    
    def generate_team_report(self, analysis):
        """Genera un reporte HTML específico para el equipo"""
        print("🌐 Generando reporte HTML para team manager...")
        
        html = f"""
        <!DOCTYPE html>
        <html>
        <head>
            <title>DevLog Interno - {self.config['project']['name']}</title>
            <style>
                body {{ 
                    font-family: 'Segoe UI', Arial, sans-serif; 
                    margin: 0; 
                    padding: 20px; 
                    background: #f5f5f5;
                }}
                .container {{ 
                    max-width: 1200px; 
                    margin: 0 auto; 
                    background: white; 
                    padding: 30px; 
                    border-radius: 10px;
                    box-shadow: 0 2px 10px rgba(0,0,0,0.1);
                }}
                .header {{ 
                    background: #2c3e50; 
                    color: white; 
                    padding: 20px; 
                    border-radius: 8px;
                    margin-bottom: 30px;
                }}
                .metrics {{ display: flex; flex-wrap: wrap; gap: 15px; margin: 20px 0; }}
                .metric-card {{ 
                    background: #ecf0f1; 
                    padding: 20px; 
                    border-radius: 8px;
                    flex: 1;
                    min-width: 150px;
                    text-align: center;
                }}
                .feature {{ border-left: 4px solid #27ae60; padding: 10px; margin: 5px 0; background: #f8fff9; }}
                .fix {{ border-left: 4px solid #e74c3c; padding: 10px; margin: 5px 0; background: #fff8f8; }}
                .improvement {{ border-left: 4px solid #f39c12; padding: 10px; margin: 5px 0; background: #fffbf0; }}
                .other {{ border-left: 4px solid #3498db; padding: 10px; margin: 5px 0; background: #f8faff; }}
                .warning {{ background: #fff3cd; border: 1px solid #ffeaa7; padding: 15px; border-radius: 5px; margin: 20px 0; }}
                .commit-grid {{ display: grid; grid-template-columns: repeat(auto-fill, minmax(300px, 1fr)); gap: 15px; }}
                .commit-card {{ padding: 15px; border-radius: 5px; border: 1px solid #ddd; }}
                code {{ background: #f4f4f4; padding: 2px 5px; border-radius: 3px; font-family: monospace; }}
            </style>
        </head>
        <body>
            <div class="container">
                <div class="header">
                    <h1>🚀 DevLog Interno - {self.config['project']['name']}</h1>
                    <p><strong>USO EXCLUSIVO DEL EQUIPO DE DESARROLLO</strong></p>
                    <p>Generado: {datetime.now().strftime("%Y-%m-%d %H:%M")}</p>
                    <p>Commits analizados: {analysis['total']}</p>
                </div>
                
                <div class="metrics">
                    <div class="metric-card">
                        <h3>📊 Total</h3>
                        <h2>{analysis['total']}</h2>
                    </div>
                    <div class="metric-card">
                        <h3>🎯 Features</h3>
                        <h2 style="color: #27ae60;">{len(analysis['features'])}</h2>
                    </div>
                    <div class="metric-card">
                        <h3>🐛 Fixes</h3>
                        <h2 style="color: #e74c3c;">{len(analysis['fixes'])}</h2>
                    </div>
                    <div class="metric-card">
                        <h3>⚡ Mejoras</h3>
                        <h2 style="color: #f39c12;">{len(analysis['improvements'])}</h2>
                    </div>
                    <div class="metric-card">
                        <h3>📄 Otros</h3>
                        <h2 style="color: #3498db;">{len(analysis['others'])}</h2>
                    </div>
                </div>
        """
        
        # Alertas para el team manager
        if analysis['total'] == 0:
            html += """
                <div class="warning">
                    <h3>❌ ALERTA CRÍTICA</h3>
                    <p>No se encontraron commits en el repositorio. Verificar:</p>
                    <ul>
                        <li>¿El directorio es un repositorio Git válido?</li>
                        <li>¿Hay commits en la rama actual?</li>
                        <li>¿Los permisos de lectura son correctos?</li>
                    </ul>
                </div>
            """
        elif analysis['total'] < 5:
            html += """
                <div class="warning">
                    <h3>⚠️ Actividad Muy Baja</h3>
                    <p>Pocos commits encontrados. Considerar:</p>
                    <ul>
                        <li>¿El equipo está trabajando en otra rama?</li>
                        <li>¿Hay problemas de integración?</li>
                        <li>¿Se están haciendo commits grandes en lugar de pequeños?</li>
                    </ul>
                </div>
            """
        elif len(analysis['fixes']) > len(analysis['features']):
            html += """
                <div class="warning">
                    <h3>⚠️ Alerta de Calidad</h3>
                    <p>Más correcciones que nuevas funcionalidades:</p>
                    <ul>
                        <li>Revisar procesos de testing</li>
                        <li>Analizar complejidad del código</li>
                        <li>Evaluar necesidad de refactorización</li>
                    </ul>
                </div>
            """
        
        # Vista resumida de commits recientes
        html += "<h2>📅 Commits Recientes (Vista Resumida)</h2>"
        html += "<div class='commit-grid'>"
        
        # Mostrar los últimos 15 commits de todos los tipos
        all_commits = (analysis['features'] + analysis['fixes'] + 
                      analysis['improvements'] + analysis['others'])
        
        # Ordenar por fecha (más reciente primero)
        all_commits.sort(key=lambda x: x['date'], reverse=True)
        
        for commit in all_commits[:15]:  # Últimos 15 commits
            type_class = commit.get('type', 'other')
            html += f"""
            <div class="commit-card {type_class}">
                <strong>{commit['date']}</strong><br>
                <code>{commit['hash']}</code><br>
                <strong>{commit['message']}</strong><br>
                <small>Por: {commit['author']}</small><br>
                <small>Tipo: {type_class.upper()}</small>
            </div>
            """
        
        html += "</div>"
        
        # Secciones detalladas por tipo
        sections = [
            ('features', '🎯 Nuevas Funcionalidades', 'feature'),
            ('fixes', '🐛 Correcciones Técnicas', 'fix'),
            ('improvements', '⚡ Mejoras Internas', 'improvement'),
            ('others', '📄 Otros Cambios', 'other')
        ]
        
        for key, title, css_class in sections:
            if analysis[key]:
                html += f'<h2>{title}</h2>'
                # Mostrar máximo 10 por categoría
                for commit in analysis[key][:10]:
                    html += f'<div class="{css_class}">'
                    html += f'<strong>{commit["date"]}</strong> '
                    html += f'<code>{commit["hash"]}</code> '
                    html += f'<span>{commit["message"]}</span>'
                    html += f'<br><small>Por: {commit["author"]}</small>'
                    if 'files' in commit and commit['files']:
                        html += f'<br><small>Archivos: {", ".join(commit["files"][:3])}'
                        if len(commit['files']) > 3:
                            html += f' ... (+{len(commit["files"]) - 3} más)'
                        html += '</small>'
                    html += '</div>'
        
        html += """
            </div>
        </body>
        </html>
        """
        return html
    
    def run(self):
        """Ejecuta el generador completo"""
        print("=" * 50)
        print("🔄 GENERADOR DE DEVLOG JTF - INICIANDO")
        print("=" * 50)
        
        try:
            # 1. Obtener TODOS los commits
            commits = self.get_all_commits()
            
            if not commits:
                print("❌ No se encontraron commits para analizar")
                return
            
            # 2. Analizar commits
            analysis = self.analyze_commit_messages(commits)
            
            # 3. Generar salidas
            doxygen_content = self.generate_doxygen_content(analysis)
            html_report = self.generate_team_report(analysis)
            
            # 4. Guardar archivos
            doxygen_file = Path(self.config["output"]["doxygen_file"])
            with open(doxygen_file, 'w', encoding='utf-8') as f:
                f.write(doxygen_content)
            
            output_dir = Path(self.config["output"]["reports_dir"])
            output_dir.mkdir(parents=True, exist_ok=True)
            
            html_file = output_dir / "team_devlog.html"
            with open(html_file, 'w', encoding='utf-8') as f:
                f.write(html_report)
            
            # 5. Mostrar resumen final
            print("=" * 50)
            print("✅ GENERACIÓN COMPLETADA EXITOSAMENTE")
            print("=" * 50)
            print(f"📊 RESUMEN FINAL:")
            print(f"   📝 Commits analizados: {analysis['total']}")
            print(f"   🎯 Features: {len(analysis['features'])}")
            print(f"   🐛 Fixes: {len(analysis['fixes'])}") 
            print(f"   ⚡ Mejoras: {len(analysis['improvements'])}")
            print(f"   📄 Otros: {len(analysis['others'])}")
            print(f"📁 ARCHIVOS GENERADOS:")
            print(f"   📄 Doxygen: {doxygen_file}")
            print(f"   🌐 HTML Team: {html_file}")
            
        except Exception as e:
            print(f"❌ ERROR CRÍTICO: {e}")
            import traceback
            traceback.print_exc()

if __name__ == "__main__":
    generator = JTFDevLogGenerator()
    generator.run()