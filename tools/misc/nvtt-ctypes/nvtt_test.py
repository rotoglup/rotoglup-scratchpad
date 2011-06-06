import unittest

import nvtt

class Test(unittest.TestCase):

  def test_nvttCreateInputOptions(self):
    outputopt = nvtt.nvttCreateInputOptions()
    nvtt.nvttDestroyInputOptions(outputopt)

  def test_nvttCreateCompressionOptions(self):
    outputopt = nvtt.nvttCreateCompressionOptions()
    nvtt.nvttDestroyCompressionOptions(outputopt)

  def test_nvttCreateOutputOptions(self):
    outputopt = nvtt.nvttCreateOutputOptions()
    nvtt.nvttDestroyOutputOptions(outputopt)

  def test_nvttCreateCompressor(self):
    compressor = nvtt.nvttCreateCompressor()
    nvtt.nvttDestroyCompressor(compressor)

if __name__ == "__main__":
  unittest.main()

