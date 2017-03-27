def FlagsForFile( filename, **kwargs ):
    return {
        'flags': [ '-x', 'c++', '-Wall', '-Wno-int-conversions',
            '-std=c++1z', '-DRUN_UNIT_TESTS'],
    }
