project(
    'mpvcmd',
    ['c'],
    version : '1.0',
    license : 'GPL-2.0',
	meson_version: '>=1.1',
    default_options : [
        'c_std=c99',
    ],
)

c_args = [
    '-Wall',
    '-Wextra',
    '-O2',
    '-D_GNU_SOURCE',
]

app_deps = [
    dependency('tinyc'),
]

app_src = [
    'main.c',
]

if get_option('need_index')
	add_project_arguments(
		'-DNEED_INDEX',
		language: 'c',
	)
endif

executable(
    meson.project_name(),
    c_args : c_args,
    dependencies : app_deps,
    sources : app_src,
    install : true,
)

