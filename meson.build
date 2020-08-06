project('gst-videoscale-ratio', 'c', version : '1.0', license : 'LGPL')

plugins_install_dir = join_paths(get_option('libdir'), 'gstreamer-1.0')

cc = meson.get_compiler('c')

gst_version = meson.project_version()

api_version = '1.0'

gst_dep = dependency('gstreamer-video-1.0')

plugin_c_args = ['-DHAVE_CONFIG_H']

cdata = configuration_data()
cdata.set_quoted('PACKAGE_VERSION', gst_version)
cdata.set_quoted('PACKAGE', 'gst-videoscale-ratio')
cdata.set_quoted('GST_LICENSE', 'LGPL')
cdata.set_quoted('GST_API_VERSION', api_version)
cdata.set_quoted('GST_PACKAGE_NAME', 'Video Scale Ratio')
cdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://github.com/aler9/gst-videoscale-ratio')
configure_file(output : 'config.h', configuration : cdata)

plugin_sources = [
    'gstvideoscaleratio.c'
    ]

plugin = library('gstvideoscaleratio',
    plugin_sources,
    c_args: plugin_c_args,
    dependencies : [gst_dep],
    install : true,
    install_dir : plugins_install_dir,
    )
