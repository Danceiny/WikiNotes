class russell-cli < Formula
  desc "Client for RussellCloud's cloud service"
  homepage "http://russellcloud.com"
  url "https://github.com/russellcloud/russell-cli.git"

  head "https://github.com/russellcloud/russell-cli.git"

#   depends_on "pip" => :build

  def install
    system "$PYTHON setup.py install"
  end

  test do
    system "#{bin}/russell", "--help"
  end
end