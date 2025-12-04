#!/usr/bin/env python3
"""
Diagnóstico del repositorio Git
"""

import subprocess
import os

def run_git(cmd):
    try:
        result = subprocess.run(["git"] + cmd, capture_output=True, text=True, check=True)
        return result.stdout.strip()
    except Exception as e:
        return f"ERROR: {e}"

print("🔍 DIAGNÓSTICO GIT")
print("=" * 40)

# 1. Verificar que estamos en repositorio Git
print("1. ¿Es repositorio Git?", os.path.exists(".git"))

# 2. Rama actual
print("2. Rama actual:", run_git(["branch", "--show-current"]))

# 3. Total de commits
print("3. Total commits:", len(run_git(["log", "--oneline"]).split('\n')))

# 4. Últimos 5 commits
print("4. Últimos 5 commits:")
print(run_git(["log", "-5", "--oneline"]))

# 5. Comando específico que usa el generador
print("5. Comando generador:")
cmd = ["log", "--all", "--pretty=format:%H|%an|%ad|%s", "--date=short", "--reverse"]
result = run_git(cmd)
print(f"   Líneas: {len(result.splitlines())}")
print(f"   Primeras 2 líneas:")
for line in result.splitlines()[:2]:
    print(f"   - {line}")