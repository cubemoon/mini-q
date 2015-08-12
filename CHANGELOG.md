# CHANGELOG

##0.1.0 (2015-0x-0x)

```
$ du -k Hello-debug.apk
2040    Hello-debug.apk
$ du -m Hello-debug.apk
2   Hello-debug.apk
```

- Remove
    1. CCHTTPRequestAndroid
    2. AssetsManager
    3. CocoStudio
    4. CCMenu*
    5. cocos_push
    6. punchbox_ad
    7. umeng*

- Refactor
    1. template: remove QMM_ROOT dependence, add lib folder (62M), soft link
    2. recompile curl with http & socket module, reduce apk size
    3. simple ui.newMenu, ui.newTTFLabelMenuItem, ui.newImageMenuItem with lua

- Add
    1. build apk tools
