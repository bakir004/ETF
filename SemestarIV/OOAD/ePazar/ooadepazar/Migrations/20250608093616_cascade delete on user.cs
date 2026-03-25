using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace ooadepazar.Migrations
{
    /// <inheritdoc />
    public partial class cascadedeleteonuser : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Artikal_AspNetUsers_KorisnikId",
                table: "Artikal");

            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_AspNetUsers_KorisnikId",
                table: "Narudzba");

            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_AspNetUsers_KurirskaSluzbaId",
                table: "Narudzba");

            migrationBuilder.AddForeignKey(
                name: "FK_Artikal_AspNetUsers_KorisnikId",
                table: "Artikal",
                column: "KorisnikId",
                principalTable: "AspNetUsers",
                principalColumn: "Id",
                onDelete: ReferentialAction.Cascade);

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_AspNetUsers_KorisnikId",
                table: "Narudzba",
                column: "KorisnikId",
                principalTable: "AspNetUsers",
                principalColumn: "Id",
                onDelete: ReferentialAction.Cascade);

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_AspNetUsers_KurirskaSluzbaId",
                table: "Narudzba",
                column: "KurirskaSluzbaId",
                principalTable: "AspNetUsers",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Artikal_AspNetUsers_KorisnikId",
                table: "Artikal");

            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_AspNetUsers_KorisnikId",
                table: "Narudzba");

            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_AspNetUsers_KurirskaSluzbaId",
                table: "Narudzba");

            migrationBuilder.AddForeignKey(
                name: "FK_Artikal_AspNetUsers_KorisnikId",
                table: "Artikal",
                column: "KorisnikId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_AspNetUsers_KorisnikId",
                table: "Narudzba",
                column: "KorisnikId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_AspNetUsers_KurirskaSluzbaId",
                table: "Narudzba",
                column: "KurirskaSluzbaId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");
        }
    }
}
