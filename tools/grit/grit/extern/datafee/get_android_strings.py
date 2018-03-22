# https://docs.python.org/2/library/xml.etree.elementtree.html
import xml.etree.ElementTree as ET
import grit.extern.FP as FP
import indent as indent

# colors: https://stackoverflow.com/questions/287871/print-in-terminal-with-colors
CRED = '\033[91m'
CYELLOW = '\33[33m'
CEND = '\033[0m'

tree_default = ET.parse('android_chrome_strings.grd')  # chrome/android/java/strings/android_chrome_strings.grd
root_default = tree_default.getroot()
ids = {}
count_default_success = 0
count_default_failure = 0
for message in root_default.iter('message'):
    try:
        id_android = message.get('name').replace('IDS_', '').lower()
        text = message.text.replace('\n', '').replace('\r', '').strip()
        id = FP.FingerPrint(text) & 0x7fffffffffffffffL
        # print id, id_android, text
        ids[id] = id_android
        count_default_success += 1
        # for ph in message.iter('ph'):
        # print ' |-->', ph.get('name')
        # count += 1
    except:
        count_default_failure += 1
        print(CRED + " Error, does not complete!" + CEND)

tree_translation = ET.parse(
    'android_chrome_strings_ru.xtb')  # chrome/android/java/strings/translations/android_chrome_strings_ru.xtb
root_translation = tree_translation.getroot()
count_translation_success = 0
count_translation_failure = 0

root_android = ET.Element('resources', {'xmlns:android': 'http://schemas.android.com/apk/res/android'})
for translation in root_translation.findall('translation'):
    id = long(translation.get('id'))
    text = translation.text
    if id in ids:
        ET.SubElement(root_android, 'string', name=ids.pop(id)).text = text
        count_translation_success += 1
    else:
        print CRED, count_translation_failure, "Error:", id, text, CEND
        count_translation_failure += 1

indent.indent(root_android) # pretty formatting for xml
tree_android = ET.ElementTree(root_android)
tree_android.write('strings.xml', encoding='utf-8', xml_declaration=True)

print(CYELLOW)
print 'REPORT START:'
print ' |-->', 'count default success:', count_default_success
print ' |-->', 'count default failure:', count_default_failure
print ' |-->', 'count default all:', count_default_failure + count_default_success
print ' |-->', 'count translation success:', count_translation_success
print ' |-->', 'count translation failure:', count_translation_failure
print ' |-->', 'count translation all:', count_translation_failure + count_translation_success
print 'REPORT END'
print(CEND)
