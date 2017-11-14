import json
from collections import OrderedDict

with open('PythonConfig.json', 'r') as json_data:
    r = json.load(json_data, object_pairs_hook=OrderedDict)
    print(json.dumps(r, indent=2))