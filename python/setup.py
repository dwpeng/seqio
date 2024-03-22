import setuptools

setuptools.setup(
    name="seqio",
    version="0.0.1",
    author="dwpeng",
    author_email="1732889554@qq.com",
    description="A package for reading and writing fasta/fastq files",
    packages=setuptools.find_namespace_packages(where="src"),
    package_dir={"": "src"},
    package_data={"seqio": ["lib/*.so", "lib/*.dylib", "lib/*.dll"]},
)
