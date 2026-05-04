import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * Gerencia registro e listagem de vendas.
 */
public class CadastroVendas {
    private List<Venda> vendas;
    private GerenciadorProdutos gerProdutos;
    private CadastroClientes cadClientes;
    private Scanner sc;

    public CadastroVendas(GerenciadorProdutos gp, CadastroClientes cc, Scanner sc) {
        this.vendas = new ArrayList<Venda>();
        this.gerProdutos = gp;
        this.cadClientes = cc;
        this.sc = sc;
    }

    public void menu() {
        int op = -1;
        while (op != 0) {
            System.out.println("\n--- MÓDULO VENDAS ---");
            System.out.println("1 - Registrar Venda");
            System.out.println("2 - Listar Vendas");
            System.out.println("0 - Voltar");
            System.out.print("Escolha: ");
            try { op = Integer.parseInt(sc.nextLine()); } catch (Exception e) { System.out.println("Entrada inválida."); continue; }

            switch (op) {
                case 1: registrarVenda(); break;
                case 2: listar(); break;
                case 0: break;
                default: System.out.println("Opção inválida.");
            }
        }
    }

    private void registrarVenda() {
        Produto p = gerProdutos.selecionarProduto();
        if (p == null) return;

        Cliente c = cadClientes.selecionarCliente();
        if (c == null) return;

        System.out.print("Quantidade: ");
        int qtd;
        try { qtd = Integer.parseInt(sc.nextLine()); } catch (Exception e) { System.out.println("Quantidade inválida."); return; }

        if (qtd <= 0) { System.out.println("Quantidade deve ser > 0."); return; }
        if (qtd > p.getQuantidade()) { System.out.println("Quantidade maior que estoque."); return; }

        // baixa no estoque
        p.setQuantidade(p.getQuantidade() - qtd);

        // marca produto como vendido (impede exclusão futura)
        gerProdutos.marcarProdutoVendido(p);

        // registra venda
        Venda v = new Venda(c, p, qtd);
        vendas.add(v);
        System.out.println("Venda registrada com sucesso. ID venda: " + v.getId());
    }

    private void listar() {
        if (vendas.isEmpty()) { System.out.println("Nenhuma venda registrada."); return; }
        System.out.println("\n--- LISTA DE VENDAS ---");
        for (Venda v : vendas) {
            v.exibir();
            System.out.println("----");
        }
    }
}
