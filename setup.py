import setuptools

extensions = [
    setuptools.Extension(
        name="_fastseqio",
        sources=["./python/fastseqio.cc", "seqio.c"],
        include_dirs=[".", "python/pybind11/include"],
        extra_compile_args=[""],
        extra_link_args=["-lz"],
    )
]

setuptools.setup(
    name="fastseqio",
    version="0.0.3",
    author="dwpeng",
    author_email="1732889554@qq.com",
    description="A package for reading and writing fasta/fastq files",
    packages=setuptools.find_namespace_packages(where="./python/src"),
    package_dir={"": "./python/src"},
    ext_modules=extensions,
)
