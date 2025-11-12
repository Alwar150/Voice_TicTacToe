#!/usr/bin/env python3
import os, yaml

BASE = "docs/devlog/items"

for item_id in sorted(os.listdir(BASE)):
    folder = os.path.join(BASE, item_id)
    if not os.path.isdir(folder): continue
    print(f"📌 {item_id}")
    for file in sorted(os.listdir(folder)):
        if not file.endswith(".yaml"): continue
        path = os.path.join(folder, file)
        with open(path) as f:
            data = yaml.safe_load(f)
        print(f"  - {file}: {data.get('status')} ({data.get('rationale')}) [{data.get('commit')}]")
        print(f"    → {data.get('file')}:{data.get('line')} [{data.get('context')}]")
    print()
