/**
 * Representa um produto no estoque.
 */
public class Produto {
    private int id;
    private String nome;
    private int quantidade;
    private double preco;
    private boolean vendidoAlgumaVez;

    public Produto(int id, String nome, int quantidade, double preco) {
        this.id = id;
        this.nome = nome;
        this.quantidade = quantidade;
        this.preco = preco;
        this.vendidoAlgumaVez = false;
    }

    public int getId() { return id; }
    public String getNome() { return nome; }
    public int getQuantidade() { return quantidade; }
    public double getPreco() { return preco; }

    public void setQuantidade(int quantidade) { this.quantidade = quantidade; }
    public void marcarComoVendido() { this.vendidoAlgumaVez = true; }
    public boolean foiVendido() { return vendidoAlgumaVez; }

    public String getStatus() {
        return (quantidade == 0) ? "Sem estoque" : "Disponível";
    }

    public String brief() {
        return "ID:" + id + " | " + nome + " | Qtd:" + quantidade + " | R$" + String.format("%.2f", preco);
    }

    @Override
    public String toString() {
        return brief() + " | " + getStatus();
    }
}
