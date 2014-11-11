Sonic Adventure DX PC Mod Loader
================================
Packaging Notes

READ THIS DOCUMENT BEFORE RELEASING A NEW VERSION!
--------------------------------------------------

When releasing a new version, the following steps should be performed.

1. Update the README.md and CHANGES.md files.
---------------------------------------------

README.md should be updated to reflect the current version.

CHANGES.md should have the changes prepended to the beginning
of the file.

2. Update the version number in CMakeLists.txt.
-----------------------------------------------

The version number during development might look something like this:

    SET(VERSION_MAJOR 3)
    SET(VERSION_MINOR 0)
    SET(VERSION_PATCH 14)
    SET(VERSION_DEVEL 1)

This indicates version 3.0.14+, with the + indicating that this is
a development build. Before tagging a release, the version number
should be bumped and VERSION_DEVEL set to 0:

    SET(VERSION_MAJOR 3)
    SET(VERSION_MINOR 1)
    SET(VERSION_PATCH 0)
    SET(VERSION_DEVEL 0)

3. Commit the changes.
-----------------------------------------------

    git commit README.md CHANGES.md CMakeLists.txt

The commit message should be something like "sadx-mod-loader 3.1".

4. Tag the release.
-----------------------------------------------

    git tag -s sadx-mod-loader-3.1

The release tag should be annotated *and* signed with a GPG key,
which is what the -s option does. If you do not have a GPG key,
you should create one and publish the public key.

5. Push the changes and package the build.
-----------------------------------------------

You can use `cpack` to package the build automatically, or use
some other manual or scripted packaging method.

6. Post-release versioning.
-----------------------------------------------

After the release, VERSION_DEVEL should be set back to 1 again.
Optionally, this should be committed immediately, but the commit can wait
until future changes are done.

The commit message for a standalone commit should be something like
"Preparing for more development."
