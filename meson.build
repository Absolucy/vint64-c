project('vint64', 'c', 'cpp',
		version: '1.0.0',
		default_options : ['c_std=c11', 'cpp_std=c++17'])
unity_proj = subproject('unity')

vint64_c = both_libraries('vint64', 'c/vint64.c',
					version : '1.0.0', soversion : '0',
					gnu_symbol_visibility : 'default')
vint64_dep = declare_dependency(include_directories : 'c',
  link_with : vint64_c)

if get_option('cpp').enabled()
	both_libraries('vint64pp', 'c++/vint64.cpp',
  			version : '1.0.0', soversion : '0',
			gnu_symbol_visibility : 'default',
			dependencies : vint64_dep)
endif

unity_dep = unity_proj.get_variable('unity_dep')

encode_test = executable('encode-test', 'tests/encode.c', dependencies : [vint64_dep, unity_dep])
test('vint64 encode', encode_test)

decode_test = executable('decode-test', 'tests/decode.c', dependencies : [vint64_dep, unity_dep])
test('vint64 decode', decode_test)
