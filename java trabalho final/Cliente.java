/**
 * Classe simples Cliente (nível básico)!
 * Se necessário, pode ser expandida para PF/PJ.
 */
public class Cliente {
    private static int proxId = 1;
    private int id;
    private String nome;
    private String documento; // CPF ou CNPJ opcional

    public Cliente(String nome, String documento) {
        this.id = proxId++;
        this.nome = nome;
        this.documento = documento;
    }

    public int getId() { return id; }
    public String getNome() { return nome; }
    public String getDocumento() { return documento; }

    public void exibir() {
        System.out.println("ID: " + id);
        System.out.println("Nome: " + nome);
        System.out.println("Documento: " + (documento == null ? "-" : documento));
    }

    @Override
    public String toString() {
        return "ID:" + id + " | " + nome + " | Doc:" + (documento == null ? "-" : documento);
    }
}
