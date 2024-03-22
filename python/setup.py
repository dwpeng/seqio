import setuptools

extensions = [
    setuptools.Extension(
        name="_seqio",
        sources=["seqio.cc", "../seqio.c"],
        include_dirs=["../"],
        language="c++",
        extra_compile_args=["-std=c++11"],
        extra_link_args=["-std=c++11", "-lz"],
    )
]

setuptools.setup(
    name="fastseqio",
    version="0.0.2",
    author="dwpeng",
    author_email="1732889554@qq.com",
    description="A package for reading and writing fasta/fastq files",
    packages=setuptools.find_namespace_packages(where="src"),
    package_dir={"": "src"},
    ext_modules=extensions,
)
