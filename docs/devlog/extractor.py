#!/usr/bin/env python3
import xml.etree.ElementTree as ET
import subprocess, os, yaml
from datetime import datetime

XML_INDEX = "./doxygen/xml/index.xml"
OUTPUT_DIR = "docs/devlog/items"

def timestamp():
    return datetime.now().strftime("%Y-%m-%dT%H-%M")

commit = subprocess.getoutput("git rev-parse --short HEAD")

tree = ET.parse(XML_INDEX)
root = tree.getroot()
os.makedirs(OUTPUT_DIR, exist_ok=True)

items = []
for member in root.findall(".//memberdef"):
    docs = member.find("detaileddescription")
    if docs is None:
        continue
    lines = "".join(docs.itertext()).splitlines()
    item = {}
    for line in lines:
        line = line.strip()
        if line.startswith("@devitem"):
            item["id"] = line.split()[1]
        elif line.startswith("@title"):
            item["title"] = line.split(" ", 1)[1]
        elif line.startswith("@status"):
            item["status"] = line.split()[1]
        elif line.startswith("@severity"):
            item["severity"] = line.split()[1]
        elif line.startswith("@owner"):
            item["owner"] = line.split()[1]
        elif line.startswith("@rationale"):
            item["rationale"] = line.split(" ", 1)[1]
        elif line.startswith("@related"):
            item.setdefault("related", []).append(line.split()[1])
    if not item:
        continue
    loc = member.find("location")
    if loc is not None:
        item["file"] = loc.get("file")
        item["line"] = int(loc.get("line", 0))
    item["context"] = member.findtext("name")
    item["commit"] = commit
    items.append(item)

for item in items:
    repo_url = subprocess.getoutput("git config --get remote.origin.url")
    if repo_url.endswith(".git"):
        repo_url = repo_url[:-4]
    item["url"] = f"{repo_url}/blob/{commit}/{item['file']}#L{item['line']}"

    folder = f"{OUTPUT_DIR}/{item['id']}"
    os.makedirs(folder, exist_ok=True)
    path = f"{folder}/{timestamp()}.yaml"
    with open(path, "w") as f:
        yaml.dump(item, f, sort_keys=False)
    with open(f"{folder}/latest.yaml", "w") as f:
        yaml.dump(item, f, sort_keys=False)

print(f"[DEVLOG] Exported {len(items)} snapshots.")
