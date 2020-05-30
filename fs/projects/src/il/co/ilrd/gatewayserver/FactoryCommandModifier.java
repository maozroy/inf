package il.co.ilrd.gatewayserver;

public interface FactoryCommandModifier {
	public void addToFactory();
	public int getVersion();
	public String getCommand();
}