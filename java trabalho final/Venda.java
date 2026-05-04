/**
 * Representa uma venda registrada.
 */
public class Venda {
    private static int proxId = 1;
    private int id;
    private Cliente cliente;
    private Produto produto;
    private int quantidade;
    private double valorTotal;

    public Venda(Cliente cliente, Produto produto, int quantidade) {
        this.id = proxId++;
        this.cliente = cliente;
        this.produto = produto;
        this.quantidade = quantidade;
        this.valorTotal = quantidade * produto.getPreco();
    }

    public int getId() { return id; }
    public Cliente getCliente() { return cliente; }
    public Produto getProduto() { return produto; }
    public int getQuantidade() { return quantidade; }
    public double getValorTotal() { return valorTotal; }

    public void exibir() {
        System.out.println("Venda ID: " + id);
        System.out.println("Cliente: " + cliente.getNome() + " (ID:" + cliente.getId() + ")");
        System.out.println("Produto: " + produto.getNome() + " (ID:" + produto.getId() + ")");
        System.out.println("Quantidade: " + quantidade);
        System.out.println("Valor total: R$ " + String.format("%.2f", valorTotal));
    }
}
