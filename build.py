import os
from conan.packager import ConanMultiPackager
import time


if __name__ == "__main__":
    # ConanPackageTools
    # See: https://github.com/conan-io/conan-package-tools/blob/develop/README.md
    builder = ConanMultiPackager()
    builder.add_common_builds()

    # Add environment variables to build definitions
    XMS_VERSION = os.environ.get('XMS_VERSION', None)
    python_target_version = os.environ.get('PYTHON_TARGET_VERSION', "3.6")
    release_python = os.environ.get('RELEASE_PYTHON', 'False')
    aquapi_username = os.environ.get('AQUAPI_USERNAME', None)
    aquapi_password = os.environ.get('AQUAPI_PASSWORD', None)
    aquapi_url = os.environ.get('AQUAPI_URL', None)

    for settings, options, env_vars, build_requires, reference in builder.items:
        # General Options
        env_vars.update({
            'XMS_VERSION': XMS_VERSION,
            'VERBOSE': 1,
            'PYTHON_TARGET_VERSION': python_target_version,
            'RELEASE_PYTHON': release_python,
            'AQUAPI_USERNAME': aquapi_username,
            'AQUAPI_PASSWORD': aquapi_password,
            'AQUAPI_URL': aquapi_url,
        })

        # Require c++11 compatibility
        if settings['compiler'] == 'gcc':
            settings.update({
                'compiler.libcxx': 'libstdc++11'
            })


    pybind_updated_builds = []
    for settings, options, env_vars, build_requires, reference in builder.items:
        # pybind option
        if (not settings['compiler'] == "Visual Studio" \
                     or int(settings['compiler.version']) > 12) \
                and settings['arch'] == "x86_64" and settings['build_type'] != 'Debug':
            pybind_options = dict(options)
            pybind_options.update({'xmsmesher:pybind': True})
            pybind_updated_builds.append([settings, pybind_options, env_vars, build_requires])

        pybind_updated_builds.append([settings, options, env_vars, build_requires])
    builder.builds = pybind_updated_builds

    xms_updated_builds = []
    for settings, options, env_vars, build_requires, reference in builder.items:
        # xms option
        if settings['compiler'] == 'Visual Studio' \
                and 'MD' in settings['compiler.runtime'] \
                and int(settings['compiler.version']) < 13:
            xms_options = dict(options)
            xms_options.update({'xmsmesher:xms': True})
            xms_updated_builds.append([settings, xms_options, env_vars, build_requires])
        xms_updated_builds.append([settings, options, env_vars, build_requires])
    builder.builds = xms_updated_builds

    testing_updated_builds = []
    for settings, options, env_vars, build_requires, reference in builder.items:
        # xms option
        if not options.get('xmsmesher:xms', False) and not options.get('xmsmesher:pybind', False):
            testing_options = dict(options)
            testing_options.update({'xmsmesher:testing': True})
            testing_updated_builds.append([settings, testing_options, env_vars, build_requires])
        testing_updated_builds.append([settings, options, env_vars, build_requires])
    builder.builds = testing_updated_builds

    builder.run()
