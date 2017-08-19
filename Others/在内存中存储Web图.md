public class WebGraphMemory{
	// 把每个URL映射为一个整数，存储在Web图中
	private Map<Integer, String> IdentifyerToURL;

	// 存储Web图的Hash表
	private Map<String, Map<String, Interger>> URLToIdentifyer;

	// 存储入度，其中整数的第一个参数是URL的ID，第二个参数存放指向这个URL链接的Map，Double表示权重
	private Map<Interger, Map<Interger,Double>> InLinks;
	// 存储出度，其中第一个参数是URL的ID，第二个参数存放网页中的超链接，Double表示权重
	private Map<Interger, Map<Interger,Double>> OutLinks;

	// 图中节点的数目
	private int nodeCount;

	// 构造函数，0个节点的构造函数
	public WebGraphMemory(){
		IdentifyerToURL = new HashMap<Integer, String>();
		URLToIdentifyer = new HashMap<String, Map<String, Integer>>();
		InLinks = new HashMap<Integer, Map<Integer, Double>>();
		OutLinks = new HashMap<Integer, Map<Integer, Double>>();
		nodeCount = 0;
	}

	// 从一个文本文件中取得节点的构造函数，每行包含一个指向关系
	// 例如： http://url1.com -》 http://url2.com 1.0
	// 表示前者包含一个超链接指向后者，并且这个超链接的权重是1.0
	public WebGraphMemory(File file) throws IOException, FileNotFoundException{
		this();
		BufferedReader reader = new BufferedReader(new FileReader(file));
		String line;
		while((line=reader.readline()) != null){
			int index1 = line.indexOf("->");
			if(index1 == -1)
				addLink(line.trim());
			else{
				String url
			}
		}
	}
}